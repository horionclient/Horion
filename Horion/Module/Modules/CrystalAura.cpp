#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::EXPLOITS, "Destroys nearby Crystals") {
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

		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			int id = ent->getEntityTypeId();
			if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= 5) {
				g_Data.getCGameMode()->attack(ent);
				g_Data.getLocalPlayer()->swingArm();
			}
		});
	}
}

void CrystalAura::onDisable() {
	this->delay = 0;
}
