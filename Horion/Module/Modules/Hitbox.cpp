#include "Hitbox.h"

Hitbox::Hitbox() : IModule(0, Category::COMBAT, "Increase an entitys hitbox size") {
	registerBoolSetting("Mobs", &this->isMobAura, this->isMobAura);
	registerFloatSetting("Height", &this->height, this->height, 1.8f, 10);
	registerFloatSetting("Width", &this->width, this->width, 0.6f, 10);
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

	if (hitboxMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < hitboxMod->range) {
		currentEntity->width = hitboxMod->width;
		currentEntity->height = hitboxMod->height;
	}
}

void Hitbox::onTick(C_GameMode* gm) {
	g_Data.forEachEntity(findTarget);
}