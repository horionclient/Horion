#include "Aimbot.h"



Aimbot::Aimbot() : IModule('M', COMBAT) // <-- keybind
{
}


Aimbot::~Aimbot()
{
}

const char* Aimbot::getModuleName()
{
	return ("Aimbot");
}
struct CompareTargetEnArray
{
	bool operator() (C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
void Aimbot::onPostRender()
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	C_EntityList* entList = g_Data.getEntityList();
	if (entList == nullptr)
		return;
	size_t listSize = entList->getListSize();

	if (listSize > 1000) {
#ifdef _DEBUG
		logF("Big ent list wtf men %i", listSize);
#endif
		return;
	}

	vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin;

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

		if (FriendList::findPlayer(currentEntity->getNameTag()->getText())) //Skip Friend
			continue;

		if (localPlayer->getEntityTypeId() != currentEntity->getEntityTypeId()) // Skip Invalid Entity
			continue;

		if (!(currentEntity->getNameTag()->getTextLength() > 0))
			continue;
		
		if (currentEntity->height < 1.5f || currentEntity->width < 0.5f || currentEntity->height > 2.1f || currentEntity->width > 0.9f)
			continue;

		// i want to hit villagers ok
//		if (localPlayer->entityType2 != currentEntity->entityType2)
//			continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());;

		if (dist < maxDist) 
		{
			targetList.push_back(currentEntity);
		}
	}

	if (targetList.size() > 0)
	{
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

		vec2_t angle = origin.CalcAngle(*targetList[0]->getPos());
		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		
		appl.div(7); // Smooth dat boi

		localPlayer->applyTurnDelta(&appl);
	}
	
}


