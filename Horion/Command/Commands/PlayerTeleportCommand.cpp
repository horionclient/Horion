#include "PlayerTeleportCommand.h"



PlayerTeleportCommand::PlayerTeleportCommand() : ICommand("playertp", "Teleports to players coordinates", "<NameOfThePlayer>")
{

}


PlayerTeleportCommand::~PlayerTeleportCommand()
{
}


bool PlayerTeleportCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	assertTrue(args->size() >= 2);
	std::string nameOfPlayer = args->at(1);
	assertTrue(nameOfPlayer.size() > 0);

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	C_EntityList* entList = g_Data.getEntityList();
	size_t listSize = entList->getListSize();
	vec3_t pos;

	if (listSize > 1000) {
		logF("Big ent list wtf men %i", listSize);
		return true;
	}

	//Loop through all our players and retrieve their information
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		if (currentEntity == 0)
			break;

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (localPlayer->getEntityTypeId() != currentEntity->getEntityTypeId()) // Skip Invalid Entity
			continue;

		if (strcmp(currentEntity->name2.getText(), nameOfPlayer.c_str()) != 0)
			continue;
		pos = currentEntity->eyePos0;

	}
	if (pos.iszero())
	{
		clientMessageF("%sPlayer not found !", GREEN);
		return true;
	}
	g_Data.getLocalPlayer()->setPos(pos);
	clientMessageF("%sTeleported!", GREEN);
	return true;
}