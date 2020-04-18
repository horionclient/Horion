#include "ScriptManager.h"

ChakraApi chakra;
ScriptManager scriptMgr;

JsValueRef ScriptManager::prepareEntityFunctions(long long runtimeId) {
	JsValueRef obj;
	EntityInfo* data = new EntityInfo(runtimeId);
	auto err = chakra.JsCreateExternalObject_(
		data, [](void* buf) {
			delete buf;
		}, &obj);

	if (err != JsNoError) {
		logF("prepareEntityFunctions error: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	chakra.defineFunction(obj, L"isValid", EntityFunctions::isValid);
	chakra.defineFunction(obj, L"getPosition", EntityFunctions::getPosition);

	return obj;
}

void ScriptManager::prepareGlobals(JsValueRef global) {
	chakra.defineFunction(global, L"log", GlobalFunctions::log);
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

	chakra.defineFunction(obj, L"isValid", Vector3Functions::isValid);

	chakra.defineValueProp(obj, L"x", chakra.toNumber(vec.x), false);
	chakra.defineValueProp(obj, L"y", chakra.toNumber(vec.y), false);
	chakra.defineValueProp(obj, L"z", chakra.toNumber(vec.z), false);

	return obj;
}

JsValueRef ScriptManager::getLocalPlayer() {
	return this->prepareEntityFunctions(-1);
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
