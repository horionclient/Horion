#include "MobAura.h"



MobAura::MobAura() : IModule('T', COMBAT) // <-- keybind
{
}


MobAura::~MobAura()
{
}

const char* MobAura::getModuleName()
{
	return ("MobAura");
}

void MobAura::onTick(C_GameMode* gm)
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	C_MobEntityList* MobList = g_Data.getLocalPlayer()->getMobList();
	size_t listSize = MobList->getListSize();

	if (listSize > 10000) {
		logF("Big ent list wtf men %i", listSize);
		return;
	}

	//Loop through all our players and retrieve their information
	float maxDist = 10;
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = MobList->get(i);

		if (currentEntity == 0)
			break;

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (localPlayer->getEntityTypeId() == currentEntity->getEntityTypeId()) // Skip Invalid Entity
			continue;

		if ((currentEntity->name2.getTextLength() > 0))
			continue;


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
	}
}

void MobAura::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

