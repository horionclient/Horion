#pragma once

#include "../ScriptManager.h"

class ModuleManagerFunctions {
private:
	static std::optional<std::shared_ptr<IModule>> getModuleFromValue(JsValueRef);

public:
	DECL_FUN(getModuleByName);
	DECL_FUN(registerModule);

	DECL_FUN(Module_getName);
	DECL_FUN(Module_toString);
	DECL_FUN(Module_isEnabled);
	DECL_FUN(Module_setEnabled);
	DECL_FUN(Module_toggle);

	DECL_FUN(JsModule_registerCallback);
};
