#include "WaypointCommand.h"

WaypointCommand::WaypointCommand() : IMCCommand("waypoint", "Manage Waypoints", "<add|remove|tp|teleport> <name> [x y z]") {
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
	std::string opt = args->at(1);
	std::string name = args->at(2);

	if (opt == "add") {
		vec3_t pos = player->currentPos;
		if (args->size() == 6) {
			pos.x = assertInt(args->at(3));
			pos.y = assertInt(args->at(4));
			pos.z = assertInt(args->at(5));
		} else if (args->size() != 3) {
			return false;
		}
		if (mod->add(name, pos.floor().add(0.5, 0, 0.5))) {
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
	} else if (opt == "tp" || opt == "teleport") {
		vec3_t* pos = mod->getWaypoint(name);
		if (pos != nullptr) {
			player->setPos(*pos);
			clientMessageF("%sTeleported to waypoint \"%s\" (%.02f, %.02f, %.02f)", GREEN, name.c_str(), pos->x, pos->y, pos->z);
		} else {
			clientMessageF("%sUnknown waypoint \"%s\"", YELLOW, name.c_str());
		}
	} else {
		return false;
	}

	return true;
}
