#include "TestCommand.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(C_Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	char* name = ent->getNameTag()->getText();
	logF("---------------");
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

void showAimedBlockInfo() {
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
	int id = block->toLegacy()->blockId;
	char* name = block->toLegacy()->name.getText();
	logF("---------------");
	logF("Block Name: %s", name);
	logF("Block ID: %d", id);
	logF("---------------");
}

ChakraApi chakra;

JsValueRef CALLBACK Echo(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::wstringstream strstream;
	for (unsigned int index = 1; index < argumentCount; index++) {
		if (index > 1) {
			strstream << L" ";
		}

		JsValueRef stringValue;
		chakra.JsConvertValueToString_(arguments[index], &stringValue);

		const wchar_t* string;
		size_t length;
		chakra.JsStringToPointer_(stringValue, &string, &length);

		strstream << string;
	}

	logF("%S", strstream.str().c_str());

	return JS_INVALID_REFERENCE;
}

void prepareContext(JsContextRef* context) {
	chakra.JsSetCurrentContext_(*context);

	JsValueRef hostObject;
	chakra.JsCreateObject_(&hostObject);

	JsValueRef globalObject;
	chakra.JsGetGlobalObject_(&globalObject);

	JsPropertyIdRef hostPropertyId;
	chakra.JsGetPropertyIdFromName_(L"Game", &hostPropertyId);

	chakra.JsSetProperty_(globalObject, hostPropertyId, hostObject, true);

	JsPropertyIdRef namePropertyId;
	chakra.JsGetPropertyIdFromName_(L"name", &namePropertyId);

	const wchar_t* str = L"Horion";
	JsValueRef naem;
	chakra.JsPointerToString_(str, wcslen(str), &naem);
	chakra.JsSetProperty_(hostObject, namePropertyId, naem, true);

	chakra.defineFunction(hostObject, L"getName", Echo);
}

bool TestCommand::execute(std::vector<std::string>* args) {
	JsRuntimeHandle runtime;
	JsContextRef context;
	JsValueRef result;
	unsigned currentSourceContext = 0;

	// Your script; try replace hello-world with something else
	std::wstring script = L"(()=>{Game.getName('gaming', 'doe');return Game.name;})()";

	chakra.JsCreateRuntime_((_JsRuntimeAttributes)((int)JsRuntimeAttributeDisableFatalOnOOM | (int)JsRuntimeAttributeAllowScriptInterrupt), nullptr, &runtime);
	chakra.JsSetRuntimeMemoryLimit_(runtime, 100000000);  // 100MB

	chakra.JsCreateContext_(runtime, &context);
	prepareContext(&context);
	chakra.JsSetCurrentContext_(context);

	chakra.JsRunScript_(script.c_str(), currentSourceContext++, L"", &result);

	JsValueRef resultJSString;
	chakra.JsConvertValueToString_(result, &resultJSString);

	const wchar_t* resultWC;
	size_t stringLength;
	chakra.JsStringToPointer_(resultJSString, &resultWC, &stringLength);

	clientMessageF("Result: %S", resultWC);

	chakra.JsSetCurrentContext_(JS_INVALID_REFERENCE);
	chakra.JsDisposeRuntime_(runtime);
	return true;
}
