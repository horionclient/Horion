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
	assertTrue(args->size() > 1); // .playertp <player>
	std::string nameOfPlayer = args->at(1);
	assertTrue(nameOfPlayer.size() > 0);

	std::transform(nameOfPlayer.begin(), nameOfPlayer.end(), nameOfPlayer.begin(), ::tolower);

	

	C_EntityList* entList = g_Data.getEntityList();
	size_t listSize = entList->getListSize();
	vec3_t pos;

	if (listSize > 5000) {
		logF("Big ent list wtf men %i", listSize);
		return true;
	}
	const char* playerName = nullptr;
	//Loop through all our players and retrieve their information
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		std::string name(currentEntity->name2.getText());

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		
		if (name.find(nameOfPlayer) == std::string::npos)
			continue;

		pos = currentEntity->eyePos0;
		playerName = name.c_str();
		break;

	}
	if (pos.iszero() || playerName == nullptr)
	{
		clientMessageF("[%sHorion%s] %sCouldn't find player: %s!",GOLD,WHITE,RED,nameOfPlayer.c_str());
		return true;
	}
	g_Data.getLocalPlayer()->setPos(pos);
	clientMessageF("[%sHorion%s] %sTeleported to %s",GOLD,WHITE,GREEN, playerName);
	return true;
}