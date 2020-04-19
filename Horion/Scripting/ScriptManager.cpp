#include "ScriptManager.h"

ChakraApi chakra;
ScriptManager scriptMgr;

void ScriptManager::prepareEntityPrototype(JsValueRef proto) {
	chakra.defineFunction(proto, L"isValid", EntityFunctions::isValid);
	chakra.defineFunction(proto, L"getPosition", EntityFunctions::getPosition);
	chakra.defineFunction(proto, L"getVelocity", EntityFunctions::getVelocity);
	chakra.defineFunction(proto, L"isOnGround", EntityFunctions::isOnGround);
	chakra.defineFunction(proto, L"getSize", EntityFunctions::getSize);
	chakra.defineFunction(proto, L"toString", EntityFunctions::toString);
	chakra.defineFunction(proto, L"getViewAngles", EntityFunctions::getViewAngles);
	chakra.defineFunction(proto, L"getYaw", EntityFunctions::getYaw);
	chakra.defineFunction(proto, L"getPitch", EntityFunctions::getPitch);
}

void ScriptManager::prepareLocalPlayerPrototype(JsValueRef proto) {
	prepareEntityPrototype(proto);

	chakra.defineFunction(proto, L"setPosition", LocalPlayerFunctions::setPosition);
	chakra.defineFunction(proto, L"setVelocity", LocalPlayerFunctions::setVelocity);
	chakra.defineFunction(proto, L"toString", LocalPlayerFunctions::toString);
}

JsValueRef ScriptManager::prepareEntity(long long runtimeId) {
	JsValueRef obj = JS_INVALID_REFERENCE;
	EntityInfo* data = new EntityInfo(runtimeId);
	auto err = chakra.JsCreateExternalObject_(
		data, [](void* buf) {
			delete buf;
		},
		&obj);

	if (err != JsNoError) {
		logF("prepareEntityFunctions error: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	if (data->isLocalPlayer)
		chakra.JsSetPrototype_(obj, LocalPlayerFunctions::prototype);
	else
		chakra.JsSetPrototype_(obj, EntityFunctions::prototype);

	return obj;
}

void ScriptManager::prepareGlobals(JsValueRef global) {
	chakra.defineFunction(global, L"log", GlobalFunctions::log);
}

void ScriptManager::prepareVector3Functions(JsValueRef global) {
	chakra.JsCreateObject_(&Vector3Functions::prototype);

	chakra.defineFunction(Vector3Functions::prototype, L"isValid", Vector3Functions::isValid);

	chakra.defineProp(Vector3Functions::prototype, L"x", Vector3Functions::getX, 0);
	chakra.defineProp(Vector3Functions::prototype, L"y", Vector3Functions::getY, 0);
	chakra.defineProp(Vector3Functions::prototype, L"z", Vector3Functions::getZ, 0);
	chakra.defineFunction(Vector3Functions::prototype, L"getX", Vector3Functions::getX);
	chakra.defineFunction(Vector3Functions::prototype, L"getY", Vector3Functions::getY);
	chakra.defineFunction(Vector3Functions::prototype, L"getZ", Vector3Functions::getZ);

	chakra.defineFunction(Vector3Functions::prototype, L"toString", Vector3Functions::toString);

	auto con = chakra.defineFunction(global, L"Vec3", Vector3Functions::constructor);
	chakra.addPropertyToObj(con, L"prototype", Vector3Functions::prototype);
}

void ScriptManager::prepareGameFunctions(JsValueRef global) {
	JsValueRef hostObject;
	chakra.JsCreateObject_(&hostObject);

	JsPropertyIdRef hostPropertyId;
	chakra.JsGetPropertyIdFromName_(L"Game", &hostPropertyId);

	chakra.JsSetProperty_(global, hostPropertyId, hostObject, true);

	chakra.defineFunction(hostObject, L"getClient", GameFunctions::getClient);
	chakra.defineFunction(hostObject, L"getLocalPlayer", GameFunctions::getLocalPlayer);
}

void ScriptManager::prepareContext(JsContextRef* ctx) {
	chakra.JsSetCurrentContext_(*ctx);

	JsValueRef globalObject;
	chakra.JsGetGlobalObject_(&globalObject);

	prepareGlobals(globalObject);
	prepareGameFunctions(globalObject);
	prepareVector3Functions(globalObject);

	chakra.JsCreateObject_(&EntityFunctions::prototype);
	prepareEntityPrototype(EntityFunctions::prototype);

	chakra.JsCreateObject_(&LocalPlayerFunctions::prototype);
	prepareLocalPlayerPrototype(LocalPlayerFunctions::prototype);
}

JsValueRef ScriptManager::prepareVector3(vec3_t vec) {
	JsValueRef obj;
	JVector3* data = new JVector3(vec);
	auto err = chakra.JsCreateExternalObject_(
		data, [](void* buf) {
			delete buf;
		},
		&obj);

	if (err != JsNoError) {
		logF("prepareVector3 error: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	chakra.JsSetPrototype_(obj, Vector3Functions::prototype);

	return obj;
}

JsValueRef ScriptManager::getLocalPlayer() {
	return this->prepareEntity(-1);
}

std::wstring ScriptManager::runScript(std::wstring script) {
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result = 0;

	chakra.JsCreateRuntime_((_JsRuntimeAttributes)((int)JsRuntimeAttributeDisableFatalOnOOM | (int)JsRuntimeAttributeAllowScriptInterrupt), nullptr, &runtime);
	chakra.JsSetRuntimeMemoryLimit_(runtime, 50000000);  // 50MB

	chakra.JsCreateContext_(runtime, &context);
	chakra.JsSetCurrentContext_(context);
	prepareContext(&context);

	auto err = chakra.JsRunScript_(script.c_str(), JS_SOURCE_CONTEXT_NONE, L"", &result);

	if (err != JsNoError) {
		logF("Script run failed: %X", err);

		bool hasException;
		chakra.JsHasException_(&hasException);
		if (hasException) {
			JsValueRef exception;
			chakra.JsGetAndClearException_(&exception);
			logF("Exception: %S", chakra.valueToString(exception).c_str());
		}
	}

	std::wstring returnString = L"No result";

	if (result != JS_INVALID_REFERENCE)
		returnString = chakra.valueToString(result);

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);

	return returnString;
}
