#pragma once

#include "../../Utils/ChakraHelper.h"
#include "../../Utils/Logger.h"
#include "../../Utils/HMath.h"

class ScriptManager;

typedef enum _ExternalDataType {
	Invalid = 0,
	EntityDataType,
	Vector3DataType
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

extern ChakraApi chakra;
extern ScriptManager scriptMgr;
#define DECL_FUN(m) static JsValueRef CALLBACK m(JsValueRef, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)

#include "GameFunctions.h"
#include "EntityFunctions.h"
#include "GlobalFunctions.h"
#include "Vector3Functions.h"

class ScriptManager {
private:
	JsValueRef prepareEntityFunctions(__int64 runtimeId);
	void prepareGlobals(JsValueRef global);
	void prepareGameFunctions(JsValueRef global);
	void prepareContext(JsContextRef* ctx);

public:
	JsValueRef prepareVector3(vec3_t vec);
	JsValueRef getLocalPlayer();
	void runScript(std::wstring);
};

