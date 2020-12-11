#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Destroys nearby Crystals") {
	//registerIntSetting("range", &range, range, 1, 10);
	registerIntSetting("Crystal range", &cRange, cRange, 1, 15);
	registerIntSetting("Place range", &eRange, eRange, 1, 5);
	registerIntSetting("Player range", &pRange, pRange, 1, 10);
	registerBoolSetting("Autoplace", &autoplace, autoplace);
	registerBoolSetting("Enhance place", &pEnhanced, pEnhanced);
	registerBoolSetting("Enhance destroy", &dEnhanced, dEnhanced);
	delay = 0;
}
CrystalAura::~CrystalAura() {
}
const char* CrystalAura::getModuleName() {
	return ("CrystalAura");
}

static std::vector<C_Entity*> targetList;

void CrystalAura::onEnable() {
	targetList.clear();
	this->delay = 0;
}

void CfindEntity(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr)
		return;
	if (curEnt == g_Data.getLocalPlayer())  // Skip Local player
		return;
	if (!curEnt->isAlive())
		return;
	if (!g_Data.getLocalPlayer()->isAlive())
		return;
	if (!g_Data.getLocalPlayer()->canAttack(curEnt, false))
		return;;
	//if (Target::VanillaAttac(curEnt, true))
	//	return;
	if (curEnt->getEntityTypeId() == 71) // endcrystal
		return;
	if (curEnt->getEntityTypeId() == 66) // falling block
		return;
	if (!Target::isValidTarget(curEnt))
		return;
	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<CrystalAura>()->pRange) {
		targetList.push_back(curEnt);
	}
}

void CrystalAura::CPlace(C_GameMode* gm, vec3_t* pos) {
	for (int x = (int)pos->x - eRange; x < pos->x + eRange; x++) {
		for (int z = (int)pos->z - eRange; z < pos->z + eRange; z++) {
			for (int y = (int)pos->y - eRange; y < pos->y + eRange; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				vec3_ti upperBlockPos = vec3_ti(x, y + 1, z);
				C_Block* block = gm->player->region->getBlock(blockPos);
				C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
				if (block != nullptr) {
					int blockId = block->toLegacy()->blockId;
					int upperBlockId = upperBlock->toLegacy()->blockId;
					if ((blockId == 49 || blockId == 7) && upperBlockId == 0//Check for awailable block 
						/* && !(x == (int)pos->x && z == (int)pos->z && y == (int)pos->y)*/){ // don't place in entety
						gm->buildBlock(&blockPos, 0);
							return;
					}
				}
			}
		}
	}
}

void CrystalAura::CDestroy(C_Entity* ent) {
	int id = ent->getEntityTypeId();
	if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) < cRange && !dEnhanced) {
		g_Data.getCGameMode()->attack(ent);
		if (!moduleMgr->getModule<NoSwing>()->isEnabled())
			g_Data.getLocalPlayer()->swingArm();
	} else if (id == 71 && dEnhanced) {
		for (auto& i : targetList)
			if (ent->getPos()->dist(*i->getPos()) < cRange) {
				g_Data.getCGameMode()->attack(ent);
				if (!moduleMgr->getModule<NoSwing>()->isEnabled())
					g_Data.getLocalPlayer()->swingArm();
				return;
			}
	}
}
bool shouldChange = false;

void CrystalAura::onTick(C_GameMode* gm) {
	if (shouldChange) {
		shouldChange = false;
	}
	this->delay++;
	targetList.clear();
	g_Data.forEachEntity(CfindEntity);
	if (this->delay >= 6) {
		this->delay = 0;
		if (autoplace && g_Data.getLocalPlayer()->getSelectedItemId() == 0x1aa) {//endcrystal
			if (pEnhanced)
				for (auto& i : targetList)
					CPlace(gm, i->getPos());
			else {
				CPlace(gm, gm->player->getPos());
			}
		}
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			moduleMgr->getModule<CrystalAura>()->CDestroy(ent);
		});
	}
}

void CrystalAura::onDisable() {
	this->delay = 0;
}