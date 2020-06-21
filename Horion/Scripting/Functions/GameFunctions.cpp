#include "GameFunctions.h"

JsValueRef CALLBACK GameFunctions::getClient(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::wstring name(L"Horion");
	JsValueRef ref;
	chakra.JsPointerToString_(name.c_str(), name.size(), &ref);
	return ref;
}

JsValueRef CALLBACK GameFunctions::getLocalPlayer(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return scriptMgr.getLocalPlayer(reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK GameFunctions::getLevel(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return reinterpret_cast<ContextObjects*>(callbackState)->levelObject;
}