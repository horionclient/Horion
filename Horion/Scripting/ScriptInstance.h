#pragma once

#include <string>
#include <filesystem>
#include <thread>
#include "../../Utils/Logger.h"
#include "ScriptManager.h"
#include <queue>
#include <mutex>
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
	std::mutex callbackMutex;
	std::condition_variable callbacksExecuted;
	std::condition_variable callbackWaiter;
	JsRuntimeHandle runtimeHandle = JS_INVALID_RUNTIME_HANDLE;
	void runPromises();
	bool checkPrintError();

public:
	ScriptInstance(std::wstring startScript);
	~ScriptInstance();

	std::wstring getStartScriptPath() {
		return this->startScriptPath;
	}

	void callCallback(JsValueRef ref) {
		if(!isRunning)
			return;
		std::lock_guard<std::mutex> lock(callbackMutex);
		this->callbackQueue.push(ref);
	}

	void callCallbackImmediate(JsValueRef ref) {
		if(!isRunning)
			return;
		std::unique_lock<std::mutex> lock(callbackMutex);
		this->callbackQueue.push(ref);
		this->callbackWaiter.notify_all();
		if(this->callbacksExecuted.wait_for(lock, std::chrono::milliseconds(20)) == std::cv_status::timeout){
			logF("Callback timeout!");
		}
	}

	void registerModule(std::shared_ptr<JsScriptModule> jsMod) {
		this->registeredModules.push_back(jsMod);
	}

	void runSync();
	void run();
};
