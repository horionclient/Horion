#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0, Category::PLAYER, "Aura for opening chests") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("EnderChests", &this->enderchests, this->enderchests);
}

ChestAura::~ChestAura() {
}

const char* ChestAura::getModuleName() {
	return ("ChestAura");
}

void ChestAura::onLevelRender() {
	if (g_Data.getLocalPlayer()->getSupplies()->inventory->isFull())
		return;

	vec3_t* pos = g_Data.getCGameMode()->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_ti pos = vec3_ti(x, y, z);
				C_Block* block = g_Data.getCGameMode()->player->region->getBlock(pos);
				if (block != nullptr && g_Data.canUseMoveKeys()) {
					auto id = g_Data.getCGameMode()->player->region->getBlock(pos)->toLegacy()->blockId;
					bool open = false;
					if (id == 54) open = true;                  // Chests
					if (id == 130 && enderchests) open = true;  // EnderCheats
					if (open)
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

void ChestAura::onDisable() {
	this->chestlist.clear();  // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
