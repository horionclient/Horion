#include "BowAimbot.h"
#include "../../../Utils/Target.h"

std::vector <C_Entity*> targetList;

BowAimbot::BowAimbot() : IModule('C', Category::COMBAT, "Aimbot, but for bows")
{
}


BowAimbot::~BowAimbot()
{
}

const char* BowAimbot::getModuleName() {
	return ("BowAimbot");
}

void findTargets(C_Entity* currentEntity,bool isRegularEntitie) {
	if (currentEntity == 0)
		return;

	if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
		return;

	if (FriendList::findPlayer(currentEntity->getNameTag()->getText())) //Skip Friend
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());;

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
		vec3_t pos = *entity->getPos();

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