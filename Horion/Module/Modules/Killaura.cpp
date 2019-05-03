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

void Killaura::onTick(C_GameMode * gm)
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	C_EntityList* entList = localPlayer->getEntityList();
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
		logF("Big ent list wtf men %i", listSize);
		return;
	}

	//Loop through all our players and retrieve their information
	float maxDist = 6;
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		if (currentEntity == 0)
			break;

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (localPlayer->entityType1 != currentEntity->entityType1) // Skip Invalid Entity
			continue;
		// i want to hit villagers ok
//		if (localPlayer->entityType2 != currentEntity->entityType2)
//			continue;

		float dist = currentEntity->eyePos1.dist(localPlayer->eyePos1);

		if (dist < maxDist) 
		{
			targetList.push_back(currentEntity);
		}
	}

	if (targetList.size() > 0)
		localPlayer->swingArm();

	C_GameMode* gameMode = localPlayer->getCGameMode();

	// Attack all entitys in targetList 
	for (int i = 0; i < targetList.size(); i++)
	{
		gameMode->attack(targetList[i]);

	}
}

