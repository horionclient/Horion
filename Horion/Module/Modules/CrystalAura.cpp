#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Destroys nearby Crystals") {
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerIntSetting("Crystal range", &this->cRange, this->cRange, 1, 15);
	registerIntSetting("Place range", &this->eRange, this->eRange, 1, 5);
	registerIntSetting("Player range", &this->pRange, this->pRange, 1, 10);
	registerBoolSetting("Auto select", &this->AutoSelect, this->AutoSelect);
	registerBoolSetting("Autoplace", &this->autoplace, this->autoplace);
	registerBoolSetting("Enhance place", &this->pEnhanced, this->pEnhanced);
	registerBoolSetting("Enhance destroy", &this->dEnhanced, this->dEnhanced);
	registerBoolSetting("preview", &this->Preview, this->Preview);
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

bool CfindEntity(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;  // falling block
	if (curEnt->getEntityTypeId() == 64) return false;  // item
	if (curEnt->getEntityTypeId() == 69) return false;  // xp orb
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<CrystalAura>()->pRange) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}

bool space = true;
vec3_t _pos;
bool CanPlaceC(vec3_ti* pos) {
	space = true;
	_pos = pos->toVec3t();
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (!space)
			return;
		if (ent->aabb.intersects(AABB(_pos, _pos.add(1.f))))
			space = false;
	});
	return space;
}

void CrystalAura::CPlace(C_GameMode* gm, vec3_t* pos) {
	if (!pEnhanced) {
#pragma warning(push)
#pragma warning(disable : 4244)
		vec3_ti blockPos = vec3_ti(pos->x, pos->y, pos->z);
		vec3_ti upperBlockPos = vec3_ti(pos->x, pos->y + 1, pos->z);
#pragma warning(pop)
		C_Block* block = gm->player->region->getBlock(blockPos);
		C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
		gm->buildBlock(&blockPos, g_Data.getClientInstance()->getPointerStruct()->blockSide);
		return;
	}
	vec3_ti bestPos;
	bool ValidPos = false;
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
					if ((blockId == 49 || blockId == 7) && upperBlockId == 0 && CanPlaceC(&blockPos)) {  //Check for awailable block
						if (!ValidPos) {
							ValidPos = true;
							bestPos = blockPos;
						} else if (blockPos.toVec3t().dist(*pos) < bestPos.toVec3t().dist(*pos)) {
							bestPos = blockPos;
						}
					}
				}
			}
		}
	}
	if (ValidPos)
		gm->buildBlock(&bestPos, 0);
}

void CrystalAura::DestroyC(C_Entity* ent, int range) {
	if (g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) < range && !dEnhanced) {
		g_Data.getCGameMode()->attack(ent);
		if (!moduleMgr->getModule<NoSwing>()->isEnabled())
			g_Data.getLocalPlayer()->swingArm();
	} else if (dEnhanced) {
		for (auto& i : targetList)
			if (ent->getPos()->dist(*i->getPos()) < range) {
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
	if (supplies == nullptr)
		supplies = g_Data.getLocalPlayer()->getSupplies();
	if (inv == nullptr)
		inv = supplies->inventory;
	targetList.clear();
	g_Data.forEachEntity(CfindEntity);
	if (this->delay == 0) {
		// place block around players?
		return;
	}

	if (this->delay == 1 && AutoSelect) {
		prevSlot = supplies->selectedHotbarSlot;
		FinishSelect = true;
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 615) {
					if (prevSlot != n)
					supplies->selectedHotbarSlot = n;
					return;
				}
			}
		}
		return;
	}
	if (this->delay == 2) {
		if (autoplace && g_Data.getLocalPlayer()->getSelectedItemId() == 615) {  //endcrystal
			if (pEnhanced)
				for (auto& i : targetList)
					CPlace(gm, i->getPos());
			else {
				auto ptr = g_Data.getClientInstance()->getPointerStruct();
				if (ptr->entityPtr == nullptr && ptr->rayHitType == 0)
					CPlace(gm, &ptr->block.toFloatVector());
			}
		}
		return;
	}
	if (this->delay == 3 && FinishSelect) {
		supplies->selectedHotbarSlot = prevSlot;
		FinishSelect = false;
		return;
	}
	if (this->delay == 4) {
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			if (ent->getEntityTypeId() != 71)
				return;
			int range;
			if (moduleMgr->getModule<CrystalAura>()->dEnhanced)
				range = moduleMgr->getModule<CrystalAura>()->cRange;
			else
				range = moduleMgr->getModule<CrystalAura>()->range;
			moduleMgr->getModule<CrystalAura>()->DestroyC(ent, range);
		});
		return;
	}
	if (this->delay >= 5) {
		this->delay = 0;
		return;
	}
}

void CrystalAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!Preview || (!pEnhanced && autoplace) ||
		g_Data.getClientInstance() == nullptr ||
		g_Data.getPtrLocalPlayer() == nullptr ||
		g_Data.getLocalPlayer() == nullptr)
		return;
	
	auto ptr = g_Data.getClientInstance()->getPointerStruct();
	if (ptr != nullptr)
		if (ptr->entityPtr == nullptr && ptr->rayHitType == 0)
			if (g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 49 ||
				g_Data.getLocalPlayer()->region->getBlock(ptr->block)->toLegacy()->blockId == 7) {
				DrawUtils::setColor(.75f, .25f, .5f, 1.f);
				DrawUtils::drawBox(ptr->block.toVec3t().add(0.f, 1.5f, 0.f),
								   ptr->block.add(1).toVec3t().add(0.f, 1.5f, 0.f), .3f);
			}
}

void CrystalAura::onDisable() {
	this->delay = 0;
}