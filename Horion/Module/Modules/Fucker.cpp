#include "Fucker.h"

Fucker::Fucker() : IModule(VK_NUMPAD9, Category::MISC, "Destroys beds around you") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
	registerBoolSetting("Chests", &this->chests, this->chests);
	registerBoolSetting("Redstone", &this->redstone, this->redstone);
	registerBoolSetting("Diamond", &this->diamond, this->diamond);
	registerBoolSetting("Emerald", &this->emerald, this->emerald);
	registerBoolSetting("Gold", &this->gold, this->gold);
	registerBoolSetting("Iron", &this->iron, this->iron);
	registerBoolSetting("Lapis", &this->lapis, this->lapis);

}

Fucker::~Fucker() {
}

const char* Fucker::getModuleName() {
	return ("Fucker");
}

void Fucker::onTick(C_GameMode* gm) {
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				bool destroy = false;
				int id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (id == 26 && this->beds) destroy = true;      // Beds
				if (id == 122 && this->eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && this->cakes) destroy = true;     // Cakes
				if (id == 54 && this->chests) destroy = true;    // Chests
				if (id == 458 && this->barrels) destroy = true;  // Barrels
				if (id == 73 && this->redstone) destroy = true;  // Redstone
				if (id == 56 && this->diamond) destroy = true;  // Diamond
				if (id == 129 && this->emerald) destroy = true;   // Emerald
				if (id == 14 && this->gold) destroy = true;  // Gold
				if (id == 15 && this->iron) destroy = true;   // Iron
				if (id == 21 && this->lapis) destroy = true;  // Lapis
				if (id == 74 && this->redstone) destroy = true;  // Redstone

				if (destroy) {
					gm->destroyBlock(&blockPos, 0);
					if (!moduleMgr->getModule<NoSwing>()->isEnabled()) 
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
			if (name.find("Treasure") != std::string::npos && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 5) {
				g_Data.getCGameMode()->attack(ent);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					g_Data.getLocalPlayer()->swingArm();
			}
		});
	}
}
