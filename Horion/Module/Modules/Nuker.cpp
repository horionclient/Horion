#include "Nuker.h"

Nuker::Nuker() : IModule(VK_NUMPAD5, Category::WORLD, "Break multiple blocks at once") {
	this->registerIntSetting("radius", &this->nukerRadius, this->nukerRadius, 1, 10);
	this->registerBoolSetting("veinminer", &this->veinMiner, this->veinMiner);
	this->registerBoolSetting("auto destroy", &this->autodestroy, this->autodestroy);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}

void Nuker::onTick(C_Player* player) {
	if (!autodestroy) return;
	vec3_ti tempPos = *player->getPos();
	for (int x = -nukerRadius; x < nukerRadius; x++) {
		for (int y = -nukerRadius; y < nukerRadius; y++) {
			for (int z = -nukerRadius; z < nukerRadius; z++) {
				tempPos.x = (int)player->getPos()->x + x;
				tempPos.y = (int)player->getPos()->y + y;
				tempPos.z = (int)player->getPos()->z + z;
				if (tempPos.y > 0 && player->region->getBlock(tempPos)->toLegacy()->material->isSolid) {
					player->getGm()->destroyBlock(&tempPos, 1);
				}
			}
		}
	}
}
