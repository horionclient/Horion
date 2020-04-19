#include "GameFunctions.h"

JsValueRef CALLBACK GameFunctions::getClient(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::wstring name(L"Horion");
	JsValueRef ref;
	chakra.JsPointerToString_(name.c_str(), name.size(), &ref);
	return ref;
}

JsValueRef CALLBACK GameFunctions::getLocalPlayer(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	
	return scriptMgr.getLocalPlayer();
}