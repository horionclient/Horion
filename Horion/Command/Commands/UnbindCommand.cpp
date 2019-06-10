#include "UnbindCommand.h"



UnbindCommand::UnbindCommand() : ICommand("unbind", "unbind modules", "<module>")
{
	registerAlias("b");
}


UnbindCommand::~UnbindCommand()
{
}

bool UnbindCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1); 

	if (moduleName.size() > 0) {
		IModule* mod = moduleMgr->getModuleByName(moduleName);
		if (mod == nullptr) {
			clientMessageF("%sCould not find Module with name: %s", RED, moduleName.c_str());
		}
		else {
			mod->setKeybind(0x0);
			clientMessageF("%sThe keybind of the %s module is now none'", GREEN, mod->getModuleName());
		}
	}
	else {
		clientMessageF("%sInvalid Command !", RED);
	}
	return true;
}