#include "FriendListCommand.h"



FriendListCommand::FriendListCommand() : ICommand("friend", "add/remove friends", "<add/remove>")
{
	registerAlias("array");
}


FriendListCommand::~FriendListCommand()
{
}


bool FriendListCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 1);
	assertTrue(args->size() >= 3);
	std::string yeet = args->at(1);
	std::string name = args->at(2);
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_Entity* currentEntity;

	C_EntityList* entList = g_Data.getEntityList();
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
		logF("Big ent list wtf men %i", listSize);
		return true;
	}

	//Loop through all our players and retrieve their information
	for (size_t i = 0; i < listSize; i++)
	{
		currentEntity = entList->get(i);

		std::string Orig_name(currentEntity->name2.getText());

		std::transform(Orig_name.begin(), Orig_name.end(), Orig_name.begin(), ::tolower);

		if (Orig_name.find(name) == std::string::npos)
			continue;

		break;

	}
	if (yeet == "add")
	{
		FriendList::addPlayerToList(currentEntity->name2.getText());
		clientMessageF("[%sHorion%s] %s%s is now your friend !", GOLD, WHITE, GREEN, currentEntity->name2.getText());
		return true;

	}
	else if (yeet == "remove" && FriendList::removePlayer(name))
	{
		clientMessageF("[%sHorion%s] %s%s is not your friend anymore :( !", GOLD, WHITE, GREEN,name.c_str());
		return true;
	}

	clientMessageF("[%sHorion%s] %sUnknow Command!", GOLD, WHITE, GREEN);
	return true;
}

