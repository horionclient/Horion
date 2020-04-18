#pragma once

#include "../../Utils/ChakraHelper.h"
#include "../../Utils/Logger.h"

extern ChakraApi chakra;
#define DECL_FUN(m) static JsValueRef CALLBACK m(JsValueRef, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState)

#include "GameFunctions.h"
#include "GlobalFunctions.h"

class ScriptManager {
private:
	void prepareGlobals(JsValueRef global);
	void prepareGameFunctions(JsValueRef global);
	void prepareContext(JsContextRef* ctx);

public:
	void runScript(std::wstring);
};

extern ScriptManager scriptMgr;