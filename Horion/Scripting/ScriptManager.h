#pragma once

#include "../../Utils/ChakraHelper.h"
#include "../../Utils/Logger.h"
#include "../../Utils/HMath.h"

typedef enum _ExternalDataType {
	Invalid = 0,
	EntityDataType,
	Vector3DataType,
	ModuleDataType,
	Vector2DataType
} ExternalDataType;

struct ExternalDataStruct {
public:
	ExternalDataType dataType;
};

struct EntityInfo : ExternalDataStruct {
	bool isLocalPlayer = false;
	long long runtimeId = 0;

	EntityInfo(long long runt) {
		this->dataType = EntityDataType;
		isLocalPlayer = runt == -1;
		runtimeId = runt;
	}
};

struct JVector3 : ExternalDataStruct {
	vec3_t vec;

	JVector3(vec3_t set) {
		this->dataType = Vector3DataType;
		this->vec = set;
	}
};

struct JVector2 : ExternalDataStruct {
	vec2_t vec;

	JVector2(vec2_t set) {
		this->dataType = Vector2DataType;
		this->vec = set;
	}
};

class IModule;

struct JModule : ExternalDataStruct {
	std::shared_ptr<IModule> modPtr;

	JModule(std::shared_ptr<IModule> mod) {
		this->dataType = ModuleDataType;
		this->modPtr = mod;
	}

	~JModule() {
		logF("JModule destroyed");
		
	}
};
class ScriptManager;

extern ChakraApi chakra;
extern ScriptManager scriptMgr;
#define DECL_FUN(m) static JsValueRef CALLBACK m(JsValueRef, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)

#define chok chakra

#define THROW(m)                \
	chok.throwTypeException(m); \
	return JS_INVALID_REFERENCE

#define THROW_IF_ERROR(code, m) \
	if (code != JsNoError) {    \
		THROW(m);				\
	}

#include "Functions/GameFunctions.h"
#include "Functions/EntityFunctions.h"
#include "Functions/GlobalFunctions.h"
#include "Functions/Vector2Functions.h"
#include "Functions/Vector3Functions.h"
#include "Functions/LocalPlayerFunctions.h"
#include "Functions/HorionFunctions.h"
#include "Functions/CommandManagerFunctions.h"
#include "Functions/ModuleManagerFunctions.h"
#include "Functions/LevelFunctions.h"
#include "Functions/DrawFunctions.h"

#include "ScriptInstance.h"

class ScriptInstance;
class JavascriptModule;

struct ContextObjects {
public:
	JsValueRef vec3Prototype = JS_INVALID_REFERENCE;
	JsValueRef vec2Prototype = JS_INVALID_REFERENCE;
	JsValueRef entityPrototype = JS_INVALID_REFERENCE;
	JsValueRef localPlayerPrototype = JS_INVALID_REFERENCE;
	JsValueRef moduleManager = JS_INVALID_REFERENCE;
	JsValueRef commandManager = JS_INVALID_REFERENCE;
	JsValueRef drawUtils = JS_INVALID_REFERENCE;
	JsValueRef levelObject = JS_INVALID_REFERENCE;
	JsValueRef modulePrototype = JS_INVALID_REFERENCE;
	JsValueRef jsModulePrototype = JS_INVALID_REFERENCE;
	ScriptInstance* scriptInstance = 0;
};

class ScriptManager {
private:

	std::vector<std::unique_ptr<ScriptInstance>> scriptInstances;

	void prepareGlobals(JsValueRef global, ContextObjects*);

	void prepareVector3Prototype(JsValueRef global, ContextObjects*);
	void prepareVector2Prototype(JsValueRef global, ContextObjects*);
	void prepareEntityPrototype(JsValueRef proto, ContextObjects* objs);
	void prepareLocalPlayerPrototype(JsValueRef proto, ContextObjects* objs);
	
	void prepareGameFunctions(JsValueRef global, ContextObjects* objs);
	void prepareHorionFunctions(JsValueRef global, ContextObjects* obj);
	void prepareDrawFunctions(JsValueRef global, ContextObjects* obj);
	void prepareCommandManagerFunctions(JsValueRef global, ContextObjects* obj);
	void prepareModuleManagerFunctions(JsValueRef global, ContextObjects* obj);
	void prepareModuleFunctions(JsValueRef proto, ContextObjects* obj);
	void prepareJsModuleFunctions(JsValueRef proto, ContextObjects* obj);
	void prepareLevelFunctions(JsValueRef global, ContextObjects* objs);

public:
	void prepareContext(JsContextRef* ctx, ContextObjects* obj);
	JsValueRef prepareEntity(__int64 runtimeId, ContextObjects* obj);
	JsValueRef prepareVector3(vec3_t vec, ContextObjects* obj);
	JsValueRef prepareVector2(vec2_t vec, ContextObjects* obj);
	JsValueRef prepareModule(std::shared_ptr<IModule> mod, ContextObjects* objs);
	JsValueRef prepareJsModule(std::shared_ptr<JavascriptModule> mod, ContextObjects* objs);
	JsValueRef getLocalPlayer(ContextObjects* obs);
	std::wstring runScript(std::wstring);
	bool importScriptFolder(std::string path);
	void unloadAllScripts();
	size_t getNumEnabledScripts();
};

