#include "Aimbot.h"



Aimbot::Aimbot() : IModule('M', Category::COMBAT, "Automatically aims at the nearest entity") {
	this->registerFloatSetting("range", &this->range, this->range, 3.f, 8.f);
	this->registerBoolSetting("Require click", &this->click, this->click);
	this->registerBoolSetting("only swords/axes", &this->sword, this->sword);
	this->registerBoolSetting("vertical", &this->vertical, this->vertical);
	this->registerFloatSetting("horizontal speed", &this->horizontalspeed, this->horizontalspeed, 10.f, 90.f);
	this->registerFloatSetting("vertical speed", &this->verticalspeed, this->verticalspeed, 10.f, 90.f);
	this->registerFloatSetting("horizontal range", &this->horizontalrange, this->horizontalrange, 20.f, 180.f);
	this->registerFloatSetting("vertical range", &this->verticalrange, this->verticalrange, 20.f, 180.f);
}

Aimbot::~Aimbot() {
}

const char* Aimbot::getModuleName() {
	return ("Aimbot");
}

struct CompareTargetEnArray {
	bool operator() (C_Entity *lhs, C_Entity *rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void Aimbot::onPostRender() {
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
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++) {
		C_Entity* currentEntity = entList->get(i);

		if (!Target::isValidTarget(currentEntity))
			continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < range)
			targetList.push_back(currentEntity);
	}

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec2_t angle = origin.CalcAngle(*targetList[0]->getPos());
		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;



		if ((appl.x < verticalrange && appl.x > -verticalrange) && (appl.y < horizontalrange && appl.y > -horizontalrange) && GameData::canUseMoveKeys()) {

			if (sword && !(localPlayer->itemId == 268 || localPlayer->itemId == 267 || localPlayer->itemId == 272 || localPlayer->itemId == 276 || localPlayer->itemId == 283 /*swords*/ || localPlayer->itemId == 271 || localPlayer->itemId == 275 || localPlayer->itemId == 279 || localPlayer->itemId == 286 || localPlayer->itemId == 258 /*axes*/))
				return;

			if (click && !g_Data.isLeftClickDown()) return;
			appl.x /= (100.f - verticalspeed);
			appl.y /= (100.f - horizontalspeed);
			if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
			if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			if (!vertical)
				appl.x = 0;
			localPlayer->applyTurnDelta(&appl);
		}
	}
}
