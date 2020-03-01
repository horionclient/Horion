#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Destroys nearby Crystals") {
	registerIntSetting("range", &range, range, 1, 10);
	registerBoolSetting("autoplace", &autoplace, autoplace);
}

CrystalAura::~CrystalAura() {
}

const char* CrystalAura::getModuleName() {
	return ("CrystalAura");
}

void CrystalAura::onEnable() {
	this->delay = 0;
}

void CrystalAura::onTick(C_GameMode* gm) {
	
	this->delay++;
	if (this->delay >= 5) {
		this->delay = 0;

		if (autoplace) {
			vec3_t* pos = gm->player->getPos();
			for (int x = (int)pos->x - range; x < pos->x + range; x++) {
				for (int z = (int)pos->z - range; z < pos->z + range; z++) {
					for (int y = (int)pos->y - range; y < pos->y + range; y++) {
						vec3_ti blockPos = vec3_ti(x, y, z);
						vec3_ti upperBlockPos = vec3_ti(x, y + 1, z);
						C_Block* block = gm->player->region->getBlock(blockPos);
						C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
						if (block != nullptr) {
							int blockId = block->toLegacy()->blockId;
							int upperBlockId = upperBlock->toLegacy()->blockId;
							if ((blockId == 49 || blockId == 7) && upperBlockId == 0) {
								gm->buildBlock(&blockPos, 0);
							}
						}
					}
				}
			}
		}

		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			int id = ent->getEntityTypeId();
			int range = moduleMgr->getModule<CrystalAura>()->range;
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
				g_Data.getCGameMode()->attack(ent);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled()) 
					g_Data.getLocalPlayer()->swingArm();
			}
		});
	}
}

void CrystalAura::onDisable() {
	this->delay = 0;
}
