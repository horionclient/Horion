#include "JavascriptModule.h"

JavascriptModule::JavascriptModule() : IModule(0, Category::CUSTOM, "") {
}

JavascriptModule::~JavascriptModule() {
	logF("deleted java moduel %s", this->getModuleName());
}

void JavascriptModule::setBackingScript(std::shared_ptr<JsScriptModule> mod) {
	this->backingScriptModule = mod;
}

std::weak_ptr<JsScriptModule> JavascriptModule::getBackingScriptModule() {
	return this->backingScriptModule;
}

const char* JavascriptModule::getModuleName() {
	auto p = this->backingScriptModule.lock();
	if (p)
		return p->getModuleNameCh();
	else
		return "INVALID_MODULE";
}

void JavascriptModule::onTick(C_GameMode* gm) {
	auto p = this->backingScriptModule.lock();
	if (!p)
		return;

	auto lock = p->lockCallbacks();
	auto callback = p->getCallback(L"onTick");
	if (callback == JS_INVALID_REFERENCE)
		return;
	
	p->getScriptInstance()->callCallback(callback);
}
void JavascriptModule::onEnable() {
	auto p = this->backingScriptModule.lock();
	if (!p)
		return;

	auto lock = p->lockCallbacks();
	auto callback = p->getCallback(L"onEnable");
	if (callback == JS_INVALID_REFERENCE)
		return;

	p->getScriptInstance()->callCallback(callback);
}
void JavascriptModule::onDisable() {
	auto p = this->backingScriptModule.lock();
	if (!p)
		return;

	auto lock = p->lockCallbacks();
	auto callback = p->getCallback(L"onDisable");
	if (callback == JS_INVALID_REFERENCE)
		return;

	p->getScriptInstance()->callCallback(callback);
}
