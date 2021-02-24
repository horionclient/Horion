#include "Nuker.h"

Nuker::Nuker() : IModule(VK_NUMPAD5, Category::WORLD, "Break multiple blocks at once") {
	this->registerIntSetting("radius", &this->nukerRadius, this->nukerRadius, 1, 10);
	this->registerBoolSetting("veinminer", &this->veinMiner, this->veinMiner);
	this->registerBoolSetting("auto destroy", &this->autodestroy, this->autodestroy);
	this->registerBoolSetting("autotool", &this->autotool, this->autotool);
}

Nuker::~Nuker() {
}

const char* Nuker::getModuleName() {
	return ("Nuker");
}

void Nuker::onTick(C_GameMode* gm) {
	if (!autodestroy) return;
	vec3_ti tempPos = *gm->player->getPos();
	for (int x = -nukerRadius; x < nukerRadius; x++) {
		for (int y = -nukerRadius; y < nukerRadius; y++) {
			for (int z = -nukerRadius; z < nukerRadius; z++) {
				tempPos.x = (int)gm->player->getPos()->x + x;
				tempPos.y = (int)gm->player->getPos()->y + y;
				tempPos.z = (int)gm->player->getPos()->z + z;
				if (tempPos.y > 0 && gm->player->region->getBlock(tempPos)->toLegacy()->material->isSolid) {
					if (autotool) {
						findTool();
					}
					gm->destroyBlock(&tempPos, 1);
				}
			}
		}
	}
}

void Nuker::findTool() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);

		if (stack->item != nullptr) {
			if ((*stack->item)->isMiningTool()) {
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}