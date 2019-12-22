#include "UnbindCommand.h"



UnbindCommand::UnbindCommand() : IMCCommand("unbind", "unbind modules", "<module | all>") {
}


UnbindCommand::~UnbindCommand() {
}

bool UnbindCommand::execute(std::vector<std::string>* args) {
	assertTrue(args->size() >= 2);
	std::string moduleName = args->at(1); 

	if (moduleName.size() > 0) {
		IModule* mod = moduleMgr->getModuleByName(moduleName);
		if (mod == nullptr) {
			if (strcmp(moduleName.c_str(), "all") == 0) {
				if (args->size() >= 3 && strcmp(args->at(2).c_str(), "force") == 0) {
					std::vector<IModule*>* modules = moduleMgr->getModuleList();
					for (auto it = modules->begin(); it != modules->end(); ++it) {
						IModule* mod = *it;
						mod->setKeybind(0x0);
					}
					clientMessageF("%sUnbound all modules!", YELLOW);
				}
				else {
					clientMessageF("%sAre you sure? This will unbind %sALL%s%s modules!", RED, BOLD, RESET, RED);
					clientMessageF("%sUse %s.unbind all force%s to unbind all modules", RED, WHITE, RED);
				}
			}
			else {
				clientMessageF("%sCould not find module with name: %s", RED, moduleName.c_str());
			}
		}
		else {
			mod->setKeybind(0x0);
			clientMessageF("%sSuccessfully unbound %s!", GREEN, mod->getModuleName());
		}
	}
	else {
		return false;
	}
	return true;
}