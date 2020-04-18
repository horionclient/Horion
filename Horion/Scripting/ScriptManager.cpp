#include "ScriptManager.h"

ChakraApi chakra;
ScriptManager scriptMgr;

void ScriptManager::prepareGlobals(JsValueRef global) {
	chakra.defineFunction(global, L"log", GlobalFunctions::log);
}

void ScriptManager::prepareGameFunctions(JsValueRef global) {
	JsValueRef hostObject;
	chakra.JsCreateObject_(&hostObject);

	JsPropertyIdRef hostPropertyId;
	chakra.JsGetPropertyIdFromName_(L"Game", &hostPropertyId);

	chakra.JsSetProperty_(global, hostPropertyId, hostObject, true);

	chakra.defineFunction(hostObject, L"getClient", GameFunctions::getClient);
}

void ScriptManager::prepareContext(JsContextRef* ctx) {
	chakra.JsSetCurrentContext_(*ctx);

	JsValueRef globalObject;
	chakra.JsGetGlobalObject_(&globalObject);

	prepareGlobals(globalObject);
	prepareGameFunctions(globalObject);
}

void ScriptManager::runScript(std::wstring script) {
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result = 0;
	unsigned currentSourceContext = 0;

	chakra.JsCreateRuntime_((_JsRuntimeAttributes)((int)JsRuntimeAttributeDisableFatalOnOOM | (int)JsRuntimeAttributeAllowScriptInterrupt), nullptr, &runtime);
	chakra.JsSetRuntimeMemoryLimit_(runtime, 50000000);  // 50MB

	chakra.JsCreateContext_(runtime, &context);
	chakra.JsSetCurrentContext_(context);
	prepareContext(&context);

	auto err = chakra.JsRunScript_(script.c_str(), currentSourceContext++, L"", &result);
	if (err != JsNoError) {
		logF("Script run failed: %X", err);

		bool hasException;
		chakra.JsHasException_(&hasException);
		if (hasException) {
			JsValueRef exception;
			chakra.JsGetAndClearException_(&exception);
			logF("Exception: %S", chakra.valueToString(exception).c_str());
		}
	}

	if (result != JS_INVALID_REFERENCE)
		logF("result: %S", chakra.valueToString(result).c_str());

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);
}
