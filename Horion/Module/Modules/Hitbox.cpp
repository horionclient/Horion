#include "Hitbox.h"


Hitbox::Hitbox() : IModule(0x0, Category::COMBAT, "Increase an entitys hitbox size")
{
	this->registerFloatSetting("Height", &this->height, this->height, 1.8f, 10);
	this->registerFloatSetting("Width", &this->width, this->width, 0.6f, 10);
}


Hitbox::~Hitbox()
{
}

const char* Hitbox::getModuleName()
{
	return ("Hitbox");
}



void findTarget(C_Entity* currentEntity,bool isRegularEntitie) {
	static Hitbox* hitboxMod = static_cast<Hitbox*>(moduleMgr->getModule<Hitbox>());
	if (hitboxMod == 0)
		hitboxMod = static_cast<Hitbox*>(moduleMgr->getModule<Hitbox>());
	else {
		if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
			return;

		if (currentEntity == 0)
			return;

		if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
			return;

		if (!Target::isValidTarget(currentEntity))
			return;
		
		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < hitboxMod->range)
		{
			currentEntity->width = hitboxMod->width;
			currentEntity->height = hitboxMod->height;
		}
	}
	
}

void Hitbox::onTick(C_GameMode* gm)
{
	if (!g_Data.isInGame())
		return;

	g_Data.forEachEntity(findTarget);

}


