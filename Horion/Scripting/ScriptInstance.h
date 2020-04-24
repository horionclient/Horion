#pragma once

#include <string>
#include <filesystem>
#include <thread>
#include "../../Utils/Logger.h"
#include "ScriptManager.h"
#include <queue>
#include "JsScriptModule.h"

class JsScriptModule;

class ScriptInstance {
private:
	std::vector<std::shared_ptr<JsScriptModule>> registeredModules;
	std::wstring startScriptPath;	
	bool isRunning = false;
	std::thread scriptThread;
	std::queue<JsValueRef> taskQueue;
	std::queue<JsValueRef> callbackQueue;
	JsRuntimeHandle runtimeHandle = JS_INVALID_RUNTIME_HANDLE;
	void runPromises();

public:
	std::mutex moduleLock;

	ScriptInstance(std::wstring startScript);
	~ScriptInstance();

	void callCallback(JsValueRef ref) {
		this->callbackQueue.push(ref);
	}

	void registerModule(std::shared_ptr<JsScriptModule> jsMod) {
		this->registeredModules.push_back(jsMod);
	}

	void runSync();
	void run();
};
