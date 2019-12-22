#include "ModulesCommand.h"
#include <algorithm>

ModulesCommand::ModulesCommand() : IMCCommand("modules", "Lists all modules", "") {
	registerAlias("mods");
}


ModulesCommand::~ModulesCommand() {
}

bool ModulesCommand::execute(std::vector<std::string>* args) {
	std::vector<IModule*>* modules = moduleMgr->getModuleList();
	g_Data.getGuiData()->displayClientMessageF("==========");
	g_Data.getGuiData()->displayClientMessageF("Modules (%i):", modules->size());
	for (auto it = modules->begin(); it != modules->end(); ++it) {
		IModule* mod = *it;
		g_Data.getGuiData()->displayClientMessageF("%s %s- %s%s", mod->getModuleName(), GRAY, ITALIC, mod->getTooltip());
	}

	return true;
}