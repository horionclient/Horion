#include "Fucker.h"

Fucker::Fucker() : IModule(VK_NUMPAD9, Category::MISC, "Destroys certain blocks around you") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
	registerBoolSetting("Chests", &this->chests, this->chests);
	registerBoolSetting("Barrels", &this->barrels, this->barrels);
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
				bool eat = false;
				auto id = gm->player->region->getBlock(blockPos)->toLegacy()->blockId;

				if (id == 26 && this->beds) destroy = true;      // Beds
				if (id == 122 && this->eggs) destroy = true;     // Dragon Eggs
				if (id == 92 && this->cakes) eat = true;         // Cakes
				if (id == 54 && this->chests) destroy = true;    // Chests
				if (id == 458 && this->barrels) destroy = true;  // Barrels

				if (destroy) {
					gm->destroyBlock(&blockPos, 0);
					g_Data.getLocalPlayer()->swingArm();
					return;
				}
				
				if (eat) {
					gm->buildBlock(&blockPos, 0);
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
				g_Data.getLocalPlayer()->swingArm();
				g_Data.getCGameMode()->attack(ent);
			}
		});
	}
}
