#include "HorionFunctions.h"

JsValueRef CALLBACK HorionFunctions::getCommandManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return CommandManagerFunctions::commandManagerObject;
}