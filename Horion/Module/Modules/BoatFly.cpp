#include "BoatFly.h"
#include "../ModuleManager.h"

BoatFly::BoatFly() : IModule(0, Category::CUSTOM, "Fly, for boats (Use arrow Keys for Virtical Movement!)") {
	this->registerFloatSetting("Fly Speed", &this->boatflyspeed, this->boatflyspeed, 1.f, 5.f);
}

BoatFly::~BoatFly() {
}

const char* BoatFly::getModuleName() {
	return ("EntityFly");
}

static std::vector<C_Entity*> targetList;

void findBoat(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (currentEntity->getEntityTypeId() != 90 && currentEntity->getEntityTypeId() != 26 && currentEntity->getEntityTypeId() != 23 && currentEntity->getEntityTypeId() != 26 && currentEntity->getEntityTypeId() != 27 && currentEntity->getEntityTypeId() != 25 && currentEntity->getEntityTypeId() != 24 && currentEntity->getEntityTypeId() != 29 && currentEntity->getEntityTypeId() != 84)
		return;

	float boatdistance = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (boatdistance< 3) {
		targetList.push_back(currentEntity);
	}
}

void BoatFly::onTick(C_GameMode* gm) {
	targetList.clear();
	g_Data.forEachEntity(findBoat);

	if (!targetList.empty()) {
		gm->player->onGround;
		float yaw = gm->player->yaw;
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		bool keyPressed = false;
		//if (GameData::canUseMoveKeys()) {
			//killaura code
			if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
				return;
			else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
				yaw += 45.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
				yaw -= 45.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
				yaw += 135.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
				yaw -= 135.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->forwardKey)) {
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->backKey)) {
				yaw += 180.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
				yaw += 90.f;
				keyPressed = true;
			} else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
				yaw -= 90.f;
				keyPressed = true;
			}
			float calcYaw = (yaw + 90) * (PI / 180);
			float calcPitch = (gm->player->pitch) * -(PI / 180);
			vec3_t moveVec;
			moveVec.x = cos(calcYaw) * boatflyspeed;
			moveVec.y = targetList[0]->velocity.y = 0;
			moveVec.z = sin(calcYaw) * boatflyspeed;
			if (keyPressed) {
				targetList[0]->lerpMotion(moveVec);
				keyPressed = false;
			} else {
				targetList[0]->velocity.x = 0;
				targetList[0]->velocity.z = 0;
			}
			//up and down
			if (g_Data.canUseMoveKeys() && !targetList.empty()) {
				if (GameData::isKeyDown(VK_DOWN)) {
					targetList[0]->velocity.y -= boatflyspeed;
				}
				if (GameData::isKeyDown(VK_UP)) {
					targetList[0]->velocity.y += boatflyspeed;
				}
			}
		//}
	}
}