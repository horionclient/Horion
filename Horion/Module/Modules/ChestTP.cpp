#include "ChestTP.h"
#include "../../../Utils/Logger.h"
#include "../../DrawUtils.h"

ChestTP::ChestTP() : IModule(0, Category::MOVEMENT, "Teleports to chests in your specified range.") {
	registerIntSetting("Range", &this->range, this->range, 1, 100);
}

ChestTP::~ChestTP() {
}

const char* ChestTP::getModuleName() {
	return "ChestTP";
}

std::vector<vec3_ti> lastPos;
void ChestTP::onEnable() {
	lastPos.clear();
}

void ChestTP::onTick(C_GameMode* gm) {

	vec3_t* pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++) {
		for (int z = (int)pos->z - range; z < pos->z + range; z++) {
			for (int y = (int)pos->y - range; y < pos->y + range; y++) {
				vec3_t pos = vec3_t(x, y, z);
				C_Block* block = gm->player->region->getBlock(pos);
				if (block != nullptr && g_Data.canUseMoveKeys()) {
					if (block->toLegacy()->blockId == 54) {
						if (!(std::find(chestlist.begin(), chestlist.end(), pos) != chestlist.end())) {
							gm->player->setPos(vec3_t(x, y+3, z));
							chestlist.push_back(pos);
							return;
						}
					}
				}
			}
		}
	}
	
}

void ChestTP::onDisable() {
	this->chestlist.clear();
}
