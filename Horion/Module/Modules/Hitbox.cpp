#include "Hitbox.h"

Hitbox::Hitbox() : IModule(0, Category::COMBAT, "Increase an entitys hitbox size") {
	registerFloatSetting("Height", &height, height, 1.8f, 10);
	registerFloatSetting("Width", &width, width, 0.6f, 10);
}

Hitbox::~Hitbox() {
}

const char* Hitbox::getModuleName() {
	return ("Hitbox");
}

void findTarget(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	
	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity == 0)
		return;

	if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < hitboxMod->range) {
		currentEntity->width = hitboxMod->width;
		currentEntity->height = hitboxMod->height;
	}
	
}

void Hitbox::onTick(C_GameMode* gm) {
	g_Data.forEachEntity(findTarget);
}
