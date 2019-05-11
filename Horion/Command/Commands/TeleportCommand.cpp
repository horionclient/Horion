#include "TeleportCommand.h"



TeleportCommand::TeleportCommand() : ICommand("tp", "Teleports to coordinates", "<X> <Y> <Z>")
{
}


TeleportCommand::~TeleportCommand()
{
}


bool TeleportCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 4);

	vec3_t pos;
	pos.x = assertFloat(args->at(1));
	pos.y = assertFloat(args->at(2));
	pos.z = assertFloat(args->at(3));

	g_Data.getLocalPlayer()->setPos(pos);
	g_Data.getGuiData()->displayClientMessageF("%sTeleported!", GREEN);
	return true;
}