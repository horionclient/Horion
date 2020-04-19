#pragma once

#include "../ScriptManager.h"
#include "../../Command/CommandMgr.h"

class CommandManagerFunctions {
public:
	static JsValueRef commandManagerObject;

	DECL_FUN(executeCommand);
};
