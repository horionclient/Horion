#include "ScriptInstance.h"

ScriptInstance::ScriptInstance(std::wstring startScript) {
	this->startScriptPath = startScript;
}

ScriptInstance::~ScriptInstance() {
	if (this->isRunning) {
		if (this->runtimeHandle != JS_INVALID_RUNTIME_HANDLE)
			chakra.JsDisableRuntimeExecution_(this->runtimeHandle);
		this->isRunning = false;
		if (this->scriptThread.joinable()) 
			this->scriptThread.join();
		
	}

	{
		auto lock = moduleMgr->lockModuleListExclusive();
		auto list = moduleMgr->getModuleList();
		for (auto it = this->registeredModules.begin(); it != this->registeredModules.end(); it++) {
			auto p = *it;
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
	}
}

void ScriptInstance::runSync() {
	std::wstring contents = Utils::wreadFileContents(this->startScriptPath);
	if (contents.size() == 0) {
		isRunning = false;
		return;
	}

	JsContextRef context;
	JsValueRef result = 0;

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
	bool hasException;
	chakra.JsHasException_(&hasException);

	if (err != JsNoError || hasException) {
		logF("Script run failed: %X", err);

		returnString = L"Error! " + std::to_wstring(err) + L", you can find a stack trace in the console";

		if (hasException) {
			JsValueRef exception;
			chakra.JsGetAndClearException_(&exception);
			logF("Exception: %S", chakra.exceptionToString(exception).c_str());
		}
	}

	returnString = chakra.valueToString(result);
	logF("Initial Script return: %S", returnString.c_str());
	this->runPromises();

	while (this->isRunning && !g_Data.shouldTerminate()) {
		Sleep(1);

		while (!this->callbackQueue.empty()) {
			auto callb = this->callbackQueue.front();
			this->callbackQueue.pop();
			chakra.JsCallFunction_(callb, &global, 1, &result);
		}
		this->runPromises();
	}

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(this->runtimeHandle);
	this->runtimeHandle = JS_INVALID_RUNTIME_HANDLE;
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
