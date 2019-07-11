#include "BowAimbot.h"

std::vector <C_Entity*> targetList;

BowAimbot::BowAimbot() : IModule('C', COMBAT)
{
}


BowAimbot::~BowAimbot()
{
}

const char* BowAimbot::getModuleName() {
	return ("BowAimbot");
}

void findTargets(C_Entity* currentEntity) {
	if (currentEntity == 0)
		return;

	if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
		return;

	if (FriendList::findPlayer(currentEntity->name2.getText())) //Skip Friend
		return;

	if (g_Data.getLocalPlayer()->getEntityTypeId() != currentEntity->getEntityTypeId()) // Skip Invalid Entity
		return;

	if (!(currentEntity->name2.getTextLength() > 0))
		return;

	if (currentEntity->aabb.upper.y - currentEntity->aabb.lower.y < 1 || currentEntity->aabb.upper.y - currentEntity->aabb.lower.y > 2)
		return;

	float dist = currentEntity->eyePos0.dist(g_Data.getLocalPlayer()->eyePos0);

	if (dist < 130)
	{
		targetList.push_back(currentEntity);
	}
}

void BowAimbot::onPostRender() {
	
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->itemId != 261) // Bow in hand?
		return;

	vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin;
	
	targetList.clear();

	g_Data.forEachEntity(findTargets);

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