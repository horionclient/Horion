#include "CommandManagerFunctions.h"

JsValueRef CALLBACK CommandManagerFunctions::executeCommand(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto commandOpt = chakra.tryGetStringFromArgs(arguments[1], argumentCount - 1);
	if (!commandOpt.has_value()) {
		THROW(L"Invalid command specified");
	}
	
	char* coolBeanBuffer = new char[commandOpt.value().size() + 2];
	sprintf_s(coolBeanBuffer, commandOpt.value().size() + 2, ".%S", commandOpt.value().c_str());
	cmdMgr->execute(coolBeanBuffer);
	delete[] coolBeanBuffer;

	return chakra.trueValue();
}