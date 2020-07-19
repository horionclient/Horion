#include "HorionFunctions.h"

JsValueRef CALLBACK HorionFunctions::getCommandManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->commandManager;
}

JsValueRef CALLBACK HorionFunctions::getModuleManager(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->moduleManager;
}

JsValueRef CALLBACK HorionFunctions::getDrawUtils(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->drawUtils;
}