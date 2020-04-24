#include "ModuleManagerFunctions.h"

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
	auto moduleNameOpt = chakra.tryGetStringFromArgs(arguments[1], argumentCount - 1);
	if (!moduleNameOpt.has_value()) {
		THROW(L"Invalid module name specified");
	}

	char* coolBeanBuffer = new char[moduleNameOpt.value().size() + 1];
	sprintf_s(coolBeanBuffer, moduleNameOpt.value().size() + 1, "%S", moduleNameOpt.value().c_str());
	auto moduleOpt = moduleMgr->getModuleByName(coolBeanBuffer);
	delete[] coolBeanBuffer;

	if (!moduleOpt.has_value())
		return chakra.nullValue();

	return scriptMgr.prepareModule(moduleOpt.value(), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK ModuleManagerFunctions::registerModule(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleNameOpt = chakra.tryGetStringFromArgs(arguments[1], argumentCount - 1);
	if (!moduleNameOpt.has_value()) {
		THROW(L"Invalid module name specified");
	}
	size_t length = moduleNameOpt.value().size();
	if (length < 3 || length > 30) {
		THROW(L"Module name should be between 3 and 30 characters long");
	}

	char* coolBeanBuffer = new char[moduleNameOpt.value().size() + 1];
	sprintf_s(coolBeanBuffer, moduleNameOpt.value().size() + 1, "%S", moduleNameOpt.value().c_str());
	std::string moduleName = coolBeanBuffer;
	delete[] coolBeanBuffer;

	auto ctxObj = reinterpret_cast<ContextObjects*>(callbackState);

	std::shared_ptr<JavascriptModule> newModule;
	std::shared_ptr<JsScriptModule> jsScriptModule;
	{
		auto lock = moduleMgr->lockModuleListExclusive();
		// Check if module already exists with that name
		auto modList = moduleMgr->getModuleList();
		bool found = false;
		{
			std::string nameCopy = moduleName;
			std::transform(nameCopy.begin(), nameCopy.end(), nameCopy.begin(), ::tolower);

			for (std::vector<std::shared_ptr<IModule>>::iterator it = modList->begin(); it != modList->end(); ++it) {
				std::shared_ptr<IModule> mod = *it;
				std::string modNameCopy = mod->getRawModuleName();
				std::transform(modNameCopy.begin(), modNameCopy.end(), modNameCopy.begin(), ::tolower);
				if (modNameCopy == nameCopy) {
					found = true;
					break;
				}
			}
		}

		if (found) {
			THROW(L"Module name is already in use");
		}

		newModule = std::make_shared<JavascriptModule>();
		jsScriptModule = std::make_shared<JsScriptModule>(moduleName, newModule, ctxObj->scriptInstance);
		newModule->setBackingScript(jsScriptModule);
		modList->push_back(newModule);
		ctxObj->scriptInstance->registerModule(jsScriptModule);
	}

	return scriptMgr.prepareJsModule(newModule, ctxObj);
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

JsValueRef CALLBACK ModuleManagerFunctions::JsModule_registerCallback(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto moduleOpt = ModuleManagerFunctions::getModuleFromValue(arguments[0]);
	if (!moduleOpt.has_value()) {
		THROW(L"Invalid module specified");
	}
	auto mod = moduleOpt.value();
	if (dynamic_cast<JavascriptModule*>(mod.get()) == nullptr) {
		THROW(L"Module is not JavascriptModule");
	}
	auto jsMod = dynamic_cast<JavascriptModule*>(mod.get());
	auto callbackNameOpt = chakra.tryGetStringFromArgs(arguments[1], argumentCount - 1);
	if (!callbackNameOpt.has_value()) {
		THROW(L"Invalid callback name specified");
	}
	auto funcOpt = chakra.tryGetFunctionFromArgs(arguments[2], argumentCount - 2);
	if (!funcOpt.has_value()) {
		THROW(L"Invalid callback function specified");
	}

	logF("name: %s", jsMod->getModuleName());
	auto weakPtr = jsMod->getBackingScriptModule();
	auto ptr = weakPtr.lock();
	if (!ptr) {
		THROW(L"Invalid module specified 2");
	}

	if (!ptr->registerCallback(callbackNameOpt.value(), funcOpt.value())) {
		THROW(L"Invalid callback name");
	}

	logF("Callback registered");

	return chakra.trueValue();
}
