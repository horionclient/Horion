#include "ChestAura.h"
#include "ChestAura.h"

ChestAura::ChestAura() : IModule(0x0, PLAYER, "Aura for opening chests")
{
}


ChestAura::~ChestAura()
{
}

const char* ChestAura::getModuleName()
{
	return ("ChestAura");
}

void ChestAura::onTick(C_GameMode* gm) {
	if (gm->player == nullptr) return;
	if (g_Data.getLocalPlayer()->getSupplies()->inventory->isFull()) return;
	vec3_t* pos = gm->player->getPos();
	for (int x = pos->x - 3; x < pos->x + 3; x++) {
	for (int z = pos->z - 3; z < pos->z + 3; z++) {
	for (int y = pos->y - 3; y < pos->y + 3; y++) {
		vec3_ti pos = vec3_ti(x, y, z);
		C_Block* block = gm->player->region->getBlock(pos);
		if (block != nullptr && g_Data.canUseMoveKeys()) {
			if (block->toLegacy()->blockId == 54) {
				if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
					g_Data.getCGameMode()->buildBlock(&pos, 0);
					chestlist.push_back(pos);
				}
			}
		}

	}
	}
	}
}

void ChestAura::onDisable()
{
	this->chestlist.clear(); // this code should be changed later, the chestlist has to be cleared when loading into a new world
}
