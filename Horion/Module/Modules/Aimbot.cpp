#include "Aimbot.h"



Aimbot::Aimbot() : IModule('M') // <-- keybind
{
}


Aimbot::~Aimbot()
{
}

std::string Aimbot::getModuleName()
{
	return std::string("Aimbot");
}
struct CompareTargetEnArray
{
	bool operator() (C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return lhs->eyePos0.dist(localPlayer->eyePos0) < rhs->eyePos0.dist(localPlayer->eyePos0); 
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

		if (FriendList::findPlayer(currentEntity->name2.getText())) //Skip Friend
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
	{
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec2_t angle = origin.CalcAngle(targetList[0]->eyePos0);


		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		
		appl.div(7); // Smooth dat boi

		localPlayer->applyTurnDelta(&appl);
	}
	
}


