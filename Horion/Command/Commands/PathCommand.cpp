#include "PathCommand.h"

#include "../../Module/ModuleManager.h"
#include "../../path/goals/JoeGoalY.h"
#include "../../path/goals/JoeGoalXZ.h"

PathCommand::PathCommand() : IMCCommand("path", "Joe path", "<y|xz> [args]"){
}
PathCommand::~PathCommand() {
}
bool PathCommand::execute(std::vector<std::string> *args) {
	assertTrue(args->size() > 1);
	static auto mod = moduleMgr->getModule<FollowPathModule>();
	if(mod->isEnabled()){
		clientMessageF("Joe is already enabled, disable joe to use this command");
		return true;
	}

	auto cmd = args->at(1);
	if(cmd == "y"){
		assertTrue(args->size() >= 2);
		int yLevel = assertInt(args->at(2));
		assertTrue(yLevel > 0 && yLevel < 256);

		mod->goal = std::make_unique<JoeGoalY>(yLevel);
		mod->setEnabled(true);

		clientMessageF("Starting search...");
		return true;
	}
	if(cmd == "xz"){
		assertTrue(args->size() >= 3);
		int x = assertInt(args->at(2));
		int z = assertInt(args->at(3));

		mod->goal = std::make_unique<JoeGoalXZ>(vec3_ti(x, 0, z));
		mod->setEnabled(true);

		clientMessageF("Starting search...");
		return true;
	}


	return false;
}
