#include "PlayerTeleportCommand.h"



PlayerTeleportCommand::PlayerTeleportCommand() : IMCCommand("playertp", "Teleports to player's coordinates. Only works if the player is in your render distance!", "<playerName>") {

}


PlayerTeleportCommand::~PlayerTeleportCommand() {
}


bool PlayerTeleportCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	assertTrue(args->size() > 1); // .playertp <player>
	std::string nameOfPlayer = args->at(1);
	assertTrue(nameOfPlayer.size() > 0);
	std::string nameOfPlayerLower = std::string(nameOfPlayer);
	std::transform(nameOfPlayerLower.begin(), nameOfPlayerLower.end(), nameOfPlayerLower.begin(), ::tolower);

	

	C_EntityList* entList = g_Data.getEntityList();
	size_t listSize = entList->getListSize();
	vec3_t pos;

	if (listSize > 5000) {
		logF("Entity list bigger than expected! %i", listSize);
		return true;
	}
	std::string playerName;
	//Loop through all our players and retrieve their information
	for (size_t i = 0; i < listSize; i++) {
		C_Entity* currentEntity = entList->get(i);

		std::string name(currentEntity->getNameTag()->getText());

		std::transform(name.begin(), name.end(), name.begin(), ::tolower);

		if (currentEntity == 0)
			break;

		if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
			continue;
		
		if (name.find(nameOfPlayerLower) == std::string::npos)
			continue;

		pos = *currentEntity->getPos();
		playerName = currentEntity->getNameTag()->getText();
		break;

	}
	if (pos.iszero()) {
		clientMessageF("[%sHorion%s] %sCouldn't find player: %s!",GOLD,WHITE,RED,nameOfPlayer.c_str());
		return true;
	}
	g_Data.getLocalPlayer()->setPos(pos);
	clientMessageF("[%sHorion%s] %sTeleported to %s",GOLD,WHITE,GREEN, playerName.c_str());
	return true;
}