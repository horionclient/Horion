#pragma once

#include "../Module/Modules/JavascriptModule.h"
#include <string>

class JavascriptModule;
class ScriptInstance;

class JsScriptModule {
private:
	ScriptInstance* scriptInstance;
	std::shared_ptr<JavascriptModule> myModule;
	std::string moduleName;
	std::mutex callbackLock;
	std::map<std::wstring, JsValueRef> callbacks;
	char moduleNameCh[30];

public:
	JsScriptModule(std::string modName, std::shared_ptr<JavascriptModule>, ScriptInstance* );
	
	std::shared_ptr<JavascriptModule> getModule() { return this->myModule; }
	inline std::string getModuleName() { return this->moduleName; }
	inline char* getModuleNameCh() { return moduleNameCh; }
	inline ScriptInstance* getScriptInstance() { return scriptInstance; }

	std::unique_lock<std::mutex> lockCallbacks() { return std::unique_lock(callbackLock); }
	JsValueRef getCallback(std::wstring callbackName);
	bool registerCallback(std::wstring callbackName, JsValueRef jsFunc);
};