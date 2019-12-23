#include "ChestAura.h"
#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0x0,Category::PLAYER, "Aura for opening chests") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
}


ChestAura::~ChestAura() {
}

const char* ChestAura::getModuleName() {
	return ("ChestAura");
}

void ChestAura::onTick(C_GameMode* gm) {
	if (gm->player == nullptr) return;
	if (g_Data.getLocalPlayer()->getSupplies()->inventory->isFull()) return;
	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
	for (int z = (int)pos->z - range; z < pos->z + range; z++) {
	for (int y = (int)pos->y - range; y < pos->y + range; y++) {
		vec3_ti pos = vec3_ti(x, y, z);
		C_Block* block = gm->player->region->getBlock(pos);
		if (block != nullptr && g_Data.canUseMoveKeys()) {
			if (block->toLegacy()->blockId == 54) {
				if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
					g_Data.getCGameMode()->buildBlock(&pos, 0);
					chestlist.push_back(pos);
					return;
				}
			}
		}

	}
	}
	}
}

void ChestAura::onDisable() {
	this->chestlist.clear(); // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
