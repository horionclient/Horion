#include "ModulesCommand.h"

#include <algorithm>
#include "../../Module/ModuleManager.h"

ModulesCommand::ModulesCommand() : IMCCommand("modules", "Lists all modules", "") {
	registerAlias("mods");
}

ModulesCommand::~ModulesCommand() {
}

bool ModulesCommand::execute(std::vector<std::string>* args) {
	auto lock = moduleMgr->lockModuleList();
	std::vector<std::shared_ptr<IModule>>* modules = moduleMgr->getModuleList();
	clientMessageF("==========");
	clientMessageF("Modules (%i):", modules->size());
	for (auto it = modules->begin(); it != modules->end(); ++it) {
		auto mod = *it;
		clientMessageF("%s %s- %s%s", mod->getModuleName(), GRAY, ITALIC, mod->getTooltip());
	}

	return true;
}
