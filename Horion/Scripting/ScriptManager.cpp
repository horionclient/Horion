#include "ScriptManager.h"

#include <filesystem>

ChakraApi chakra;
ScriptManager scriptMgr;

JsValueRef ScriptManager::prepareEntity(long long runtimeId, ContextObjects* objs) {
	JsValueRef obj = JS_INVALID_REFERENCE;
	if(g_Data.isInGame() && g_Data.getLocalPlayer()->entityRuntimeId == runtimeId)
		runtimeId = -1;
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
		chakra.JsSetPrototype_(obj, objs->localPlayerPrototype);
	else
		chakra.JsSetPrototype_(obj, objs->entityPrototype);

	return obj;
}

void ScriptManager::prepareGlobals(JsValueRef global) {
	chakra.defineFunction(global, L"log", GlobalFunctions::log);
}

void ScriptManager::prepareVector3Prototype(JsValueRef global, ContextObjects* obj) {
	chakra.JsCreateObject_(&obj->vec3Prototype);
	chakra.JsAddRef_(obj->vec3Prototype, 0);

	chakra.defineFunction(obj->vec3Prototype, L"isValid", Vector3Functions::isValid, obj);

	chakra.defineProp(obj->vec3Prototype, L"x", Vector3Functions::getX, 0);
	chakra.defineProp(obj->vec3Prototype, L"y", Vector3Functions::getY, 0);
	chakra.defineProp(obj->vec3Prototype, L"z", Vector3Functions::getZ, 0);
	chakra.defineFunction(obj->vec3Prototype, L"getX", Vector3Functions::getX, obj);
	chakra.defineFunction(obj->vec3Prototype, L"getY", Vector3Functions::getY, obj);
	chakra.defineFunction(obj->vec3Prototype, L"getZ", Vector3Functions::getZ, obj);

	chakra.defineFunction(obj->vec3Prototype, L"toString", Vector3Functions::toString, obj);

	chakra.defineFunction(obj->vec3Prototype, L"add", Vector3Functions::add, obj);
	chakra.defineFunction(obj->vec3Prototype, L"sub", Vector3Functions::sub, obj);

	auto con = chakra.defineFunction(global, L"Vec3", Vector3Functions::constructor, obj);
	chakra.addPropertyToObj(con, L"prototype", obj->vec3Prototype);
}

void ScriptManager::prepareEntityPrototype(JsValueRef proto, ContextObjects* objs) {
	chakra.defineFunction(proto, L"isValid", EntityFunctions::isValid, objs);
	chakra.defineFunction(proto, L"getPosition", EntityFunctions::getPosition, objs);
	chakra.defineFunction(proto, L"getInterpolatedPosition", EntityFunctions::getInterpolatedPosition, objs);
	chakra.defineFunction(proto, L"getVelocity", EntityFunctions::getVelocity, objs);
	chakra.defineFunction(proto, L"isOnGround", EntityFunctions::isOnGround, objs);
	chakra.defineFunction(proto, L"isInvisible", EntityFunctions::isInvisible, objs);
	chakra.defineFunction(proto, L"isInWater", EntityFunctions::isInWater, objs);
	chakra.defineFunction(proto, L"isInLava", EntityFunctions::isInLava, objs);
	chakra.defineFunction(proto, L"isSneaking", EntityFunctions::isSneaking, objs);
	chakra.defineFunction(proto, L"getSize", EntityFunctions::getSize, objs);
	chakra.defineFunction(proto, L"toString", EntityFunctions::toString, objs);
	chakra.defineFunction(proto, L"getViewAngles", EntityFunctions::getViewAngles, objs);
	chakra.defineFunction(proto, L"getYaw", EntityFunctions::getYaw, objs);
	chakra.defineFunction(proto, L"getPitch", EntityFunctions::getPitch, objs);
}

void ScriptManager::prepareLocalPlayerPrototype(JsValueRef proto, ContextObjects* objs) {
	prepareEntityPrototype(proto, objs);

	chakra.defineFunction(proto, L"setPosition", LocalPlayerFunctions::setPosition, objs);
	chakra.defineFunction(proto, L"setVelocity", LocalPlayerFunctions::setVelocity, objs);
	chakra.defineFunction(proto, L"toString", LocalPlayerFunctions::toString, objs);
	chakra.defineFunction(proto, L"setViewAngles", LocalPlayerFunctions::setViewAngles, objs);
	chakra.defineFunction(proto, L"setIsOnGround", LocalPlayerFunctions::setIsOnGround, objs);
}

void ScriptManager::prepareGameFunctions(JsValueRef global, ContextObjects* objs) {
	JsValueRef gameObject;
	chakra.JsCreateObject_(&gameObject);

	chakra.addPropertyToObj(global, L"Game", gameObject);

	chakra.defineFunction(gameObject, L"getClient", GameFunctions::getClient, objs);
	chakra.defineFunction(gameObject, L"getLocalPlayer", GameFunctions::getLocalPlayer, objs);
	chakra.defineFunction(gameObject, L"getLevel", GameFunctions::getLevel, objs);
}

void ScriptManager::prepareHorionFunctions(JsValueRef global, ContextObjects* obj) {
	this->prepareDrawFunctions(global, obj);
	this->prepareCommandManagerFunctions(global, obj);
	this->prepareModuleManagerFunctions(global, obj);

	JsValueRef horionObject;
	chakra.JsCreateObject_(&horionObject);

	chakra.addPropertyToObj(global, L"Horion", horionObject);

	chakra.defineFunction(horionObject, L"getCommandManager", HorionFunctions::getCommandManager, obj);
	chakra.defineFunction(horionObject, L"getModuleManager", HorionFunctions::getModuleManager, obj);
	chakra.defineFunction(horionObject, L"getDrawUtils", HorionFunctions::getDrawUtils, obj);
}

void ScriptManager::prepareDrawFunctions(JsValueRef global, ContextObjects* objs) {
	chakra.JsCreateObject_(&objs->drawUtils);
	chakra.JsAddRef_(objs->drawUtils, 0);

	chakra.defineFunction(objs->drawUtils, L"drawLine3d", DrawFunctions::drawLine3d, objs);
	chakra.defineFunction(objs->drawUtils, L"drawLinestrip3d", DrawFunctions::drawLinestrip3d, objs);
	chakra.defineFunction(objs->drawUtils, L"setColor", DrawFunctions::setColor, objs);
}

void ScriptManager::prepareCommandManagerFunctions(JsValueRef global, ContextObjects* objs) {
	chakra.JsCreateObject_(&objs->commandManager);
	chakra.JsAddRef_(objs->commandManager, 0);

	chakra.defineFunction(objs->commandManager, L"executeCommand", CommandManagerFunctions::executeCommand, objs);
}

void ScriptManager::prepareModuleManagerFunctions(JsValueRef global, ContextObjects* obj) {
	chakra.JsCreateObject_(&obj->modulePrototype);
	chakra.JsAddRef_(obj->modulePrototype, 0);
	prepareModuleFunctions(obj->modulePrototype, obj);

	chakra.JsCreateObject_(&obj->jsModulePrototype);
	chakra.JsAddRef_(obj->jsModulePrototype, 0);
	prepareJsModuleFunctions(obj->jsModulePrototype, obj);

	chakra.JsCreateObject_(&obj->moduleManager);
	chakra.JsAddRef_(obj->moduleManager, 0);

	chakra.defineFunction(obj->moduleManager, L"getModuleByName", ModuleManagerFunctions::getModuleByName, obj);

	if (obj->scriptInstance != nullptr) {
		// enable module register support

		chakra.defineFunction(obj->moduleManager, L"registerModule", ModuleManagerFunctions::registerModule, obj);
	}
}

void ScriptManager::prepareModuleFunctions(JsValueRef proto, ContextObjects* obj) {
	chakra.defineFunction(proto, L"getName", ModuleManagerFunctions::Module_getName, obj);
	chakra.defineFunction(proto, L"toString", ModuleManagerFunctions::Module_toString, obj);
	chakra.defineFunction(proto, L"isEnabled", ModuleManagerFunctions::Module_isEnabled, obj);
	chakra.defineFunction(proto, L"setEnabled", ModuleManagerFunctions::Module_setEnabled, obj);
	chakra.defineFunction(proto, L"toggle", ModuleManagerFunctions::Module_toggle, obj);
}

void ScriptManager::prepareJsModuleFunctions(JsValueRef proto, ContextObjects* obj) {
	prepareModuleFunctions(proto, obj);

	chakra.defineFunction(proto, L"registerCallback", ModuleManagerFunctions::JsModule_registerCallback, obj);
}

void ScriptManager::prepareContext(JsContextRef* ctx, ContextObjects* obj) {
	chakra.JsSetCurrentContext_(*ctx);

	JsValueRef globalObject;
	chakra.JsGetGlobalObject_(&globalObject);

	prepareGlobals(globalObject);
	prepareHorionFunctions(globalObject, obj);
	prepareGameFunctions(globalObject, obj);
	prepareVector3Prototype(globalObject, obj);
	prepareLevelFunctions(globalObject, obj);

	chakra.JsCreateObject_(&obj->entityPrototype);
	chakra.JsAddRef_(obj->entityPrototype, 0);
	prepareEntityPrototype(obj->entityPrototype, obj);

	chakra.JsCreateObject_(&obj->localPlayerPrototype);
	chakra.JsAddRef_(obj->localPlayerPrototype, 0);
	prepareLocalPlayerPrototype(obj->localPlayerPrototype, obj);
}

JsValueRef ScriptManager::prepareVector3(vec3_t vec, ContextObjects* objs) {
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

	chakra.JsSetPrototype_(obj, objs->vec3Prototype);

	return obj;
}

JsValueRef ScriptManager::prepareModule(std::shared_ptr<IModule> mod, ContextObjects* objs) {
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

	err = chakra.JsSetPrototype_(obj, objs->modulePrototype);

	if (err != JsNoError) {
		logF("prepareModule error2: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	return obj;
}

JsValueRef ScriptManager::prepareJsModule(std::shared_ptr<JavascriptModule> mod, ContextObjects* objs) {
	JsValueRef obj = 0;
	JModule* data = new JModule(mod);
	auto err = chakra.JsCreateExternalObject_(
		data, [](void* buf) {
			delete buf;
		},
		&obj);

	if (err != JsNoError) {
		logF("prepareModule2 error: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	err = chakra.JsSetPrototype_(obj, objs->jsModulePrototype);

	if (err != JsNoError) {
		logF("prepareModule2 error2: %X", err);
		JsValueRef null;
		chakra.JsGetNullValue_(&null);
		return null;
	}

	return obj;
}

void ScriptManager::prepareLevelFunctions(JsValueRef global, ContextObjects* objs) {
	chakra.JsCreateObject_(&objs->levelObject);
	chakra.JsAddRef_(objs->levelObject, 0);

	chakra.defineFunction(objs->levelObject, L"isValid", LevelFunctions::isValid, objs);
	chakra.defineFunction(objs->levelObject, L"getAllEntities", LevelFunctions::getAllEntities, objs);
	chakra.defineFunction(objs->levelObject, L"getAllTargetEntities", LevelFunctions::getAllTargetEntities, objs);
}

JsValueRef ScriptManager::getLocalPlayer(ContextObjects* obs) {
	return this->prepareEntity(-1, obs);
}

std::wstring ScriptManager::runScript(std::wstring script) {
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result = 0;

	chakra.JsCreateRuntime_((_JsRuntimeAttributes)((int)JsRuntimeAttributeDisableFatalOnOOM | (int)JsRuntimeAttributeAllowScriptInterrupt | (int)JsRuntimeAttributeDisableBackgroundWork), nullptr, &runtime);
	chakra.JsSetRuntimeMemoryLimit_(runtime, 50000000);  // 50MB

	chakra.JsCreateContext_(runtime, &context);
	chakra.JsSetCurrentContext_(context);
	ContextObjects objs;
	prepareContext(&context, &objs);

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
			logF("Exception: %S", chakra.exceptionToString(exception).c_str());
		}
	}

	if (result != JS_INVALID_REFERENCE)
		returnString = chakra.valueToString(result);

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);

	return returnString;
}

bool ScriptManager::importScriptFolder(std::string path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			wchar_t fname[100];
			wchar_t ext[7];
			auto err = _wsplitpath_s(entry.path().c_str(), 0, 0, 0, 0, fname, 100, ext, 7);
			if (err != 0)
				continue;
			std::wstring fileName = std::wstring(fname) + ext;
			if (fileName == L"start.js") {
				// check if script with that name is active already
				for (auto it = this->scriptInstances.begin(); it != this->scriptInstances.end(); it++) {
					auto startScriptPath = (*it)->getStartScriptPath();
					if (entry.path() == startScriptPath) {
						logF("Script already loaded!!!");
						return false;
					}
				}
				auto script = std::make_unique<ScriptInstance>(entry.path());
				script->run();
				this->scriptInstances.push_back(std::move(script));
				
				return true;
			}
		}
	}
	logF("Could not find start script! Create a file called start.js in your folder.");
	return false;
}

void ScriptManager::unloadAllScripts() {
	this->scriptInstances.clear();
}
size_t ScriptManager::getNumEnabledScripts() {
	return this->scriptInstances.size();
}
