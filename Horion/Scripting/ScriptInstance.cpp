#include "ScriptInstance.h"

ScriptInstance::ScriptInstance(std::wstring startScript) {
	this->startScriptPath = startScript;
}

ScriptInstance::~ScriptInstance() {
	if (this->isRunning) {
		if (this->runtimeHandle != JS_INVALID_RUNTIME_HANDLE)
			chakra.JsDisableRuntimeExecution_(this->runtimeHandle);
		this->isRunning = false;
	}
	if (this->scriptThread.joinable())
		this->scriptThread.join();

	{
		auto lock = moduleMgr->lockModuleListExclusive();
		auto list = moduleMgr->getModuleList();
		for (const auto& p : this->registeredModules) {
			auto pos = std::find(list->begin(), list->end(), p->getModule());
			if (pos == list->end()) {
				logF("couldn't find module???");
				continue;
			}
			list->erase(pos);
		}
		this->registeredModules.clear();
	}
}

void CALLBACK promiseContinuationCallback(JsValueRef task, void* callbackState) {
	// Save promise task in taskQueue.
	auto q = (std::queue<JsValueRef>*)callbackState;
	q->push(task);
	chakra.JsAddRef_(task, nullptr);
}

void ScriptInstance::runPromises() {
	JsValueRef global;
	chakra.JsGetGlobalObject_(&global);
	JsValueRef result;

	while (!taskQueue.empty()) {
		JsValueRef task = taskQueue.front();
		taskQueue.pop();
		chakra.JsCallFunction_(task, &global, 1, &result);
		chakra.JsRelease_(task, nullptr);
		this->checkPrintError();
	}
}

void ScriptInstance::runSync() {
	std::wstring contents = Utils::wreadFileContents(this->startScriptPath);
	if (contents.empty()) {
		isRunning = false;
		return;
	}

	JsContextRef context;
	JsValueRef result = nullptr;

	chakra.JsCreateRuntime_((_JsRuntimeAttributes)((int)JsRuntimeAttributeDisableFatalOnOOM | (int)JsRuntimeAttributeAllowScriptInterrupt | (int)JsRuntimeAttributeDisableBackgroundWork), nullptr, &this->runtimeHandle);
	if (!isRunning) {
		chakra.JsDisposeRuntime_(this->runtimeHandle);
		this->runtimeHandle = JS_INVALID_RUNTIME_HANDLE;
		return;
	}
	chakra.JsSetRuntimeMemoryLimit_(this->runtimeHandle, 50000000);  // 50MB

	chakra.JsCreateContext_(this->runtimeHandle, &context);
	chakra.JsSetCurrentContext_(context);
	ContextObjects obj;
	obj.scriptInstance = this;
	scriptMgr.prepareContext(&context, &obj);
	JsValueRef global;
	chakra.JsGetGlobalObject_(&global);

	auto err = chakra.JsRunScript_(contents.c_str(), JS_SOURCE_CONTEXT_NONE, L"", &result);

	std::wstring returnString = L"No result";

	if (err != JsNoError || this->checkPrintError()) {
		logF("Script run failed: %X", err);

		returnString = L"Error! " + std::to_wstring(err) + L", you can find a stack trace in the console";
	}

	returnString = chakra.valueToString(result);
	logF("Initial Script return: %S", returnString.c_str());
	this->runPromises();

	while (this->isRunning && !GameData::shouldTerminate()) {
		{
			std::unique_lock<std::mutex> lk(callbackMutex);
			this->callbackWaiter.wait_for(lk, std::chrono::milliseconds(1));

			while (!this->callbackQueue.empty()) {
				auto callb = this->callbackQueue.front();
				this->callbackQueue.pop();
				chakra.JsCallFunction_(callb, &global, 1, &result);
				this->checkPrintError();
			}
		}
		this->callbacksExecuted.notify_all();

		this->runPromises();
	}

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(this->runtimeHandle);
	this->runtimeHandle = JS_INVALID_RUNTIME_HANDLE;
	this->isRunning = false;
}

void ScriptInstance::run() {
	if (this->isRunning) {
		logF("tried to run a script that's already running");
		return;
	}
	auto thisPtr = this;
	this->isRunning = true;
	this->scriptThread = std::thread([thisPtr]() {
		thisPtr->runSync();
		logF("Script Execution finished");
	});
}
bool ScriptInstance::checkPrintError() {
	bool hasException;
	chakra.JsHasException_(&hasException);

	if (hasException) {
		JsValueRef exception;
		chakra.JsGetAndClearException_(&exception);
		auto exceptionStr = chakra.exceptionToString(exception);
		logF("Exception: %S", exceptionStr.c_str());
		if(exceptionStr.size() > 70){
			logF("check the logfile for exceptions");
		}
	}

	return hasException;
}
