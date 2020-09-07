#include "WaypointCommand.h"

WaypointCommand::WaypointCommand() : IMCCommand("waypoint", "Manage Waypoints", "<add|remove> <name>") {
	registerAlias("wp");
}

WaypointCommand::~WaypointCommand() {
}

bool WaypointCommand::execute(std::vector<std::string>* args) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	assertTrue(player != nullptr);
	assertTrue(args->size() > 2);

	static auto mod = moduleMgr->getModule<Waypoints>();
	if (mod == nullptr)
		return true;

	auto opt = args->at(1);
	std::string name = args->at(2);
	assertTrue(name.length() > 0);

	if (opt == "add") {
		if (mod->add(name, player->currentPos.floor().add(0.5, player->eyePos0.y - player->currentPos.y, 0.5))) {
			clientMessageF("%sSuccessfully added waypoint \"%s\"", GREEN, name.c_str());
		} else {
			clientMessageF("%sWaypoint \"%s\" already exists", YELLOW, name.c_str());
		}
	} else if (opt == "remove") {
		if (mod->remove(name)) {
			clientMessageF("%sRemoved waypoint \"%s\"", RED, name.c_str());
		} else {
			clientMessageF("%sUnknown waypoint \"%s\"", YELLOW, name.c_str());
		}
	} else {
		return false;
	}

	return true;
}
