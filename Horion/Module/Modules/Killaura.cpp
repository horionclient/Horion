#include "Killaura.h"



Killaura::Killaura() : IModule('P') // <-- keybind
{
}


Killaura::~Killaura()
{
}

std::string Killaura::getModuleName()
{
	return std::string("Killaura");
}

void Killaura::onTick(C_GameMode* gm)
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	C_EntityList* entList = g_Data.getEntityList();
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
		logF("Big ent list wtf men %i", listSize);
		return;
	}

	//Loop through all our players and retrieve their information
	float maxDist = 10;
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		if (currentEntity == 0)
			break;
		
		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (FriendList::findPlayer(currentEntity->name2.getText())) // Skip Friend
			continue;

		if (localPlayer->getEntityTypeId() != currentEntity->getEntityTypeId()) // Skip Invalid Entity
			continue;

		if (!(currentEntity->name2.getTextLength() > 0))
			continue;

		if (currentEntity->aabb.upper.y - currentEntity->aabb.lower.y < 1 || currentEntity->aabb.upper.y - currentEntity->aabb.lower.y > 2)
			continue;

		// i want to hit villagers ok
//		if (localPlayer->entityType2 != currentEntity->entityType2)
//			continue;

		float dist = currentEntity->eyePos0.dist(localPlayer->eyePos0);

		if (dist < maxDist) 
		{
			targetList.push_back(currentEntity);
		}
	}

	if (targetList.size() > 0)
		localPlayer->swingArm();

	// Attack all entitys in targetList 
	for (int i = 0; i < targetList.size(); i++)
	{
		g_Data.getCGameMode()->attack(targetList[i]);
		//copy = targetList[i];
		//delay = 0;
	}
}

void Killaura::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr) {
		this->setEnabled(false);
		return;
	}
	delay = 100;
}

