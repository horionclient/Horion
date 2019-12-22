#include "PanicCommand.h"



PanicCommand::PanicCommand() : IMCCommand("panic", "Disables all Modules", "") {
}


PanicCommand::~PanicCommand() {
}

bool PanicCommand::execute(std::vector<std::string>* args) {
	std::vector<IModule*>* modules = moduleMgr->getModuleList();
	
	for (auto it = modules->begin(); it != modules->end(); ++it) {
		IModule* mod = *it;
		mod->setEnabled(false);
	}
	g_Data.getGuiData()->displayClientMessageF("%sDisabled all Modules!", YELLOW);
	return true;
}
