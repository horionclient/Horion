#include "ScriptManager.h"

#include <filesystem>

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
	chakra.defineFunction(proto, L"setViewAngles", LocalPlayerFunctions::setViewAngles);
	chakra.defineFunction(proto, L"setIsOnGround", LocalPlayerFunctions::setIsOnGround);
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

void ScriptManager::prepareVector3Prototype(JsValueRef global) {
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
	JsValueRef gameObject;
	chakra.JsCreateObject_(&gameObject);

	chakra.addPropertyToObj(global, L"Game", gameObject);

	chakra.defineFunction(gameObject, L"getClient", GameFunctions::getClient);
	chakra.defineFunction(gameObject, L"getLocalPlayer", GameFunctions::getLocalPlayer);
}

void ScriptManager::prepareHorionFunctions(JsValueRef global) {
	this->prepareCommandManagerFunctions(global);
	this->prepareModuleManagerFunctions(global);
	
	JsValueRef horionObject;
	chakra.JsCreateObject_(&horionObject);

	chakra.addPropertyToObj(global, L"Horion", horionObject);

	chakra.defineFunction(horionObject, L"getCommandManager", HorionFunctions::getCommandManager);
	chakra.defineFunction(horionObject, L"getModuleManager", HorionFunctions::getModuleManager);
}

void ScriptManager::prepareCommandManagerFunctions(JsValueRef global) {
	chakra.JsCreateObject_(&CommandManagerFunctions::commandManagerObject);

	chakra.defineFunction(CommandManagerFunctions::commandManagerObject, L"executeCommand", CommandManagerFunctions::executeCommand);
}

void ScriptManager::prepareModuleManagerFunctions(JsValueRef global) {
	prepareModuleFunctions(global);

	chakra.JsCreateObject_(&ModuleManagerFunctions::moduleManagerObject);

	chakra.defineFunction(ModuleManagerFunctions::moduleManagerObject, L"getModuleByName", ModuleManagerFunctions::getModuleByName);
}

void ScriptManager::prepareModuleFunctions(JsValueRef global) {
	chakra.JsCreateObject_(&ModuleManagerFunctions::modulePrototype);

	chakra.defineFunction(ModuleManagerFunctions::modulePrototype, L"getName", ModuleManagerFunctions::Module_getName);
	chakra.defineFunction(ModuleManagerFunctions::modulePrototype, L"toString", ModuleManagerFunctions::Module_toString);
	chakra.defineFunction(ModuleManagerFunctions::modulePrototype, L"isEnabled", ModuleManagerFunctions::Module_isEnabled);
	chakra.defineFunction(ModuleManagerFunctions::modulePrototype, L"setEnabled", ModuleManagerFunctions::Module_setEnabled);
	chakra.defineFunction(ModuleManagerFunctions::modulePrototype, L"toggle", ModuleManagerFunctions::Module_toggle);
}

void ScriptManager::prepareContext(JsContextRef* ctx) {
	chakra.JsSetCurrentContext_(*ctx);

	JsValueRef globalObject;
	chakra.JsGetGlobalObject_(&globalObject);

	prepareGlobals(globalObject);
	prepareHorionFunctions(globalObject);
	prepareGameFunctions(globalObject);
	prepareVector3Prototype(globalObject);

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

JsValueRef ScriptManager::prepareModule(std::shared_ptr<IModule> mod) {
	JsValueRef obj = 0;
	JModule* data = new JModule(mod);
	auto err = chakra.JsCreateExternalObject_(
		data, [](void* buf) {
			delete buf;
		},
		&obj);

	if (err != JsNoError) {
		logF("prepareModule error: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	err = chakra.JsSetPrototype_(obj, ModuleManagerFunctions::modulePrototype);

	if (err != JsNoError) {
		logF("prepareModule error2: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

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

	std::wstring returnString = L"No result";
	bool hasException;
	chakra.JsHasException_(&hasException);

	if (err != JsNoError || hasException) {
		logF("Script run failed: %X", err);

		returnString = L"Error! " + std::to_wstring(err) + L", you can find a stack trace in the console";

		if (hasException) {
			JsValueRef exception;
			chakra.JsGetAndClearException_(&exception);
			JsValueRef stack;
			chakra.getProperty(exception, L"stack", &stack);
			if (chakra.isNullOrUndefined(stack))
				logF("Exception: %S", chakra.valueToString(exception).c_str());
			else 
				logF("Exception stack: %S", chakra.valueToString(stack).c_str());
		}
	}

	if (result != JS_INVALID_REFERENCE)
		returnString = chakra.valueToString(result);

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);

	return returnString;
}

void ScriptManager::importScriptFolder(std::string path) {
	//logF("Importing script from path: %s", path.c_str());
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			wchar_t fname[100];
			wchar_t ext[7];
			auto err = _wsplitpath_s(entry.path().c_str(), 0, 0, 0, 0, fname, 100, ext, 7);
			if (err != 0)
				continue;
			std::wstring fileName = std::wstring(fname) + ext;
			if (fileName == L"start.js") {
				
				std::wstring contents = Utils::wreadFileContents(entry.path());
				
				auto ret = this->runScript(contents);
				logF("Script returned: %S", ret.c_str());
				return;
			}
		}
	}
	logF("Could not find start script! Create a file called start.js in your folder.");
}
