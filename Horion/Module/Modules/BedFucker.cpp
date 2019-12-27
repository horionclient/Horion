#include "BedFucker.h"



BedFucker::BedFucker() : IModule(VK_NUMPAD9, Category::EXPLOITS, "Destroys beds around you")
{
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
	registerBoolSetting("Chests", &this->chests, this->chests);
	registerBoolSetting("Barrels", &this->barrels, this->barrels);
}


BedFucker::~BedFucker()
{
}

const char* BedFucker::getModuleName()
{
	return ("Fucker");
}

void BedFucker::onEnable(){

	this->delay = 0;

}

void BedFucker::onTick(C_GameMode* gm) {

	this->delay++;

	if (this->delay >= 5) {

		this->delay = 0;

		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
		for (int y = (int)pos->y - range; y < pos->y + range; y++) {
			vec3_ti blockPos = vec3_ti(x, y, z);
			bool destroy = false;

			if (g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId == 26 && this->beds) destroy = true; // Beds
			if (g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId == 122 && this->eggs) destroy = true; // Dragon Eggs
			if (g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId == 92 && this->cakes)  destroy = true; // Cakes
			if (g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId == 54 && this->chests) destroy = true; // Chests
			if (g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId == 458 && this->barrels) destroy = true; // Barrels

			if (destroy) {
				g_Data.getCGameMode()->destroyBlock(&blockPos, 0);
				g_Data.getLocalPlayer()->swingArm();
				return;
			}
		}
		}
		}

		if (this->treasures) {
			g_Data.forEachEntity([](C_Entity* ent, bool b) {
				std::string name = ent->getNameTag()->getText();
				int id = ent->getEntityTypeId();
				if (name.find("Treasure") != std::string::npos && id == 319 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 5) {
					g_Data.getCGameMode()->attack(ent);
					g_Data.getLocalPlayer()->swingArm();
				}
				});
		}
	}
}

void BedFucker::onDisable() {

	this->delay = 0;

}
