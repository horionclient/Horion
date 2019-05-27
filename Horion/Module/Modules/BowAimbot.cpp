#include "BowAimbot.h"



BowAimbot::BowAimbot() : IModule('C')
{
}


BowAimbot::~BowAimbot()
{
}

std::string BowAimbot::getModuleName() {
	return std::string("BowAimbot");
}

void BowAimbot::onPostRender() {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->itemId != 261) // Bow in hand?
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
	float maxDist = 130;
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

		float dist = currentEntity->eyePos0.dist(localPlayer->eyePos0);

		if (dist < maxDist)
		{
			targetList.push_back(currentEntity);
		}
	}

	if (targetList.size() > 0)
	{
		vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin; // TODO: sort list
		C_Entity* entity = targetList[0];
		vec3_t pos = entity->eyePos0;

		pos = pos.sub(origin);

		float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
		float len = pos.magnitudexz();
		float g = 0.006f; // nukkit = 0.012, some servers use different calculus
		float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
		float pitch = DEG_RAD * -(atanf((1 - sqrtf(tmp)) / (g * len)));
		if (pitch < 89 && pitch > -89) {
			vec2_t angles = vec2_t(pitch, yaw);

			vec2_t appl = angles.sub(localPlayer->viewAngles).normAngles();
			appl.x = -appl.x;

			appl.div(7); // Smooth dat boi

			localPlayer->applyTurnDelta(&appl);
		}
	}
}