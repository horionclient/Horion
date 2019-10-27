#include "TeleportCommand.h"



TeleportCommand::TeleportCommand() : IMCCommand("tp", "Teleports to coordinates", "<X> <Y> <Z>")
{
	registerAlias("teleport");
	registerAlias("setpos");
}


TeleportCommand::~TeleportCommand()
{
}


bool TeleportCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 4);

	vec3_t pos;
	pos.x = assertFloat(args->at(1))+(float)0.5;
	pos.y = assertFloat(args->at(2))+(float)g_Data.getLocalPlayer()->height;
	clientMessageF("%s%f", GREEN, (float)g_Data.getLocalPlayer()->height);
	pos.z = assertFloat(args->at(3))+(float)0.5;

	g_Data.getLocalPlayer()->setPos(pos);
	clientMessageF("%sTeleported!", GREEN);
	return true;
}