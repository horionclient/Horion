#include "ToggleCommand.h"



ToggleCommand::ToggleCommand() : IMCCommand("toggle", "Toggles a module", "<module>") {
	registerAlias("t");
}


ToggleCommand::~ToggleCommand() {
}

bool ToggleCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1);

	assertTrue(moduleName.size() > 0);


	IModule* mod = moduleMgr->getModuleByName(moduleName);
	if (mod == nullptr) {
		clientMessageF("%sCould not find Module with name: %s", RED, moduleName.c_str());
	}
	else {
		if (mod->isFlashMode()) {
			clientMessageF("%sModule cannot be toggled!", RED);
		}
		mod->toggle();
		clientMessageF("%s%s is now %s", GREEN, mod->getModuleName(), mod->isEnabled() ? "Enabled" : "Disabled");
	}

	return true;
}
