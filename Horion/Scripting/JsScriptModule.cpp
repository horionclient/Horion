#include "JsScriptModule.h"

JsScriptModule::JsScriptModule(std::string modName, std::shared_ptr<JavascriptModule> mod, ScriptInstance* sc) : moduleName(modName), myModule(mod), scriptInstance(sc) {
	strcpy_s(this->moduleNameCh, moduleName.c_str());
}

JsValueRef JsScriptModule::getCallback(std::wstring callbackName) {
	if (callbacks.find(callbackName) == callbacks.end()) {
		return JS_INVALID_REFERENCE;
	}
	return callbacks[callbackName];
}

bool JsScriptModule::registerCallback(std::wstring callbackName, JsValueRef jsFunc) {
	if (callbackName != L"onTick" && callbackName != L"onEnable" && callbackName != L"onDisable" && callbackName != L"onAttack" && callbackName != L"onRender" && callbackName != L"onRender2d")
		return false;

	chakra.JsAddRef_(jsFunc, 0);
	if (callbacks.find(callbackName) != callbacks.end()) {
		// Remove old
		auto old = callbacks[callbackName];
		if (old != JS_INVALID_REFERENCE)
			chakra.JsRelease_(old, 0);
	}

	callbacks[callbackName] = jsFunc;

	return true;
}
