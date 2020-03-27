#include "RelativeTeleportCommand.h"

RelativeTeleportCommand::RelativeTeleportCommand() : IMCCommand("relativeteleport", "Teleports to coordinates relative to the Player", "<X> <Y> <Z>") {
	registerAlias("reltp");
	registerAlias("rtp");
	registerAlias("vclip");
}

RelativeTeleportCommand::~RelativeTeleportCommand() {
}

bool RelativeTeleportCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 4);

	vec3_t pPos = g_Data.getLocalPlayer()->eyePos0;

	vec3_t pos;
	pos.x = assertFloat(args->at(1)) + pPos.x;
	pos.y = assertFloat(args->at(2)) + pPos.y;
	pos.z = assertFloat(args->at(3)) + pPos.z;

	g_Data.getLocalPlayer()->setPos(pos);
	clientMessageF("%sTeleported!", GREEN);
	return true;
}
