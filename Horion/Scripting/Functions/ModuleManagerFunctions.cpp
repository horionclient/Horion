#include "ModuleManagerFunctions.h"

JsValueRef ModuleManagerFunctions::moduleManagerObject;
JsValueRef ModuleManagerFunctions::modulePrototype;

std::optional<std::shared_ptr<IModule>> ModuleManagerFunctions::getModuleFromValue(JsValueRef ref) {
	if (ref == JS_INVALID_REFERENCE)
		return std::optional<std::shared_ptr<IModule>>();
	JsValueType type;
	auto err = chakra.JsGetValueType_(ref, &type);
	if (type != JsObject || err != JsNoError)
		return std::optional<std::shared_ptr<IModule>>();

	bool hasExternalData = false;
	err = chakra.JsHasExternalData_(ref, &hasExternalData);
	if (!hasExternalData || err != JsNoError)
		return std::optional<std::shared_ptr<IModule>>();

	JModule* modInfo;
	err = chakra.JsGetExternalData_(ref, reinterpret_cast<void**>(&modInfo));
	if (modInfo->dataType != ModuleDataType || err != JsNoError)
		return std::optional<std::shared_ptr<IModule>>();

	return std::optional<std::shared_ptr<IModule>>(modInfo->modPtr);
}

JsValueRef CALLBACK ModuleManagerFunctions::getModuleByName(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleNameOpt = chakra.tryGetStringFromArgs(&arguments[1], argumentCount - 1);
	if (!moduleNameOpt.has_value()) {
		THROW(L"Invalid module name specified");
	}

	char* coolBeanBuffer = new char[moduleNameOpt.value().size() + 1];
	sprintf_s(coolBeanBuffer, moduleNameOpt.value().size() + 1, "%S", moduleNameOpt.value().c_str());
	auto moduleOpt = moduleMgr->getModuleByName(coolBeanBuffer);
	delete[] coolBeanBuffer;

	if (!moduleOpt.has_value())
		return chakra.nullValue();

	return scriptMgr.prepareModule(moduleOpt.value());
}

JsValueRef CALLBACK ModuleManagerFunctions::Module_getName(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}

	auto wstr = Utils::stringToWstring(moduleOpt.value()->getModuleName());
	JsValueRef ref;
	chakra.JsPointerToString_(wstr.c_str(), wstr.size(), &ref);

	return ref;
}

JsValueRef CALLBACK ModuleManagerFunctions::Module_toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}

	auto wstr = Utils::stringToWstring(moduleOpt.value()->getModuleName());
	auto strComp = std::wstring(L"Module(name=") + wstr + L")";
	JsValueRef ref;
	chakra.JsPointerToString_(strComp.c_str(), strComp.size(), &ref);
	return ref;
}

JsValueRef CALLBACK ModuleManagerFunctions::Module_isEnabled(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}

	return chakra.toBoolean(moduleOpt.value()->isEnabled());
}

JsValueRef CALLBACK ModuleManagerFunctions::Module_setEnabled(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}

	auto bolOpt = chakra.tryGetBoolFromArgs(&arguments[1], argumentCount - 1);
	if (!bolOpt.has_value()) {
		THROW(L"Invalid boolean specified");
	}

	moduleOpt.value()->setEnabled(bolOpt.value());

	return chakra.trueValue();
}

JsValueRef CALLBACK ModuleManagerFunctions::Module_toggle(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}

	moduleOpt.value()->toggle();

	return chakra.trueValue();
}
