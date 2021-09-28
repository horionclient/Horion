#include "FightBot.h"

FightBot::FightBot() : IModule(0, Category::COMBAT, "Moves and attacks at the closest player") {
	this->registerFloatSetting("Range", &this->range, this->range, 3.f, 8.f);
	this->registerFloatSetting("Reach", &this->reach, this->reach, 2.f, 7.f);
	this->registerIntSetting("Delay", &this->delay, this->delay, 0, 10);
}

FightBot::~FightBot() {
}

const char* FightBot::getModuleName() {
	return ("FightBot");
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void FightBot::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr) return;
	
	C_EntityList* entList = g_Data.getEntityList();
	if (entList == nullptr) return;
	size_t listSize = entList->getListSize();

	if (listSize > 1000) return;

	vec3_t origin = g_Data.getClientInstance()->levelRenderer->origin;

	static std::vector<C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++) {
		C_Entity* currentEntity = entList->get(i);

		if (!Target::isValidTarget(currentEntity)) continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < range) targetList.push_back(currentEntity);
	}

	if (targetList.size() > 0) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec2_t angle = origin.CalcAngle(*targetList[0]->getPos());
		vec2_t appl = angle.sub(localPlayer->viewAngles).normAngles();
		appl.x = -appl.x;
		
		if (GameData::canUseMoveKeys()) {
			if (appl.x >= 1 || appl.x <= -1) appl.div(abs(appl.x));
			if (appl.y >= 1 || appl.y <= -1) appl.div(abs(appl.y));
			localPlayer->applyTurnDelta(&appl);
			
			g_Data.getClientInstance()->getMoveTurnInput()->forward = true;
			
			float dist = (*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
			Odelay++;
			if (Odelay >= delay && reach >= dist) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetList[0]);
			}
			if (Odelay >= delay) Odelay = 0;
		}
	}
}

void FightBot::onDisable() {
	g_Data.getClientInstance()->getMoveTurnInput()->forward = false;
}
