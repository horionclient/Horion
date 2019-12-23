#include "NameTags.h"



NameTags::NameTags() : IModule(0x0, Category::VISUAL, "Shows better nametags above players that can be seen from a lot more far aways") {
	this->registerBoolSetting("Display Health", &this->displayHealth, this->displayHealth);
	this->registerBoolSetting("Use Unicode font", &this->useUnicodeFont, this->useUnicodeFont);
}


NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNameTags(C_Entity* ent, bool isRegularEntitie) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	static NameTags* NameTagsMod = moduleMgr->getModule<NameTags>();

	if (ent != localPlayer) {

		if (ent->timeSinceDeath > 0)
			return;
		if (Target::isValidTarget(ent) && NameTagsMod != nullptr)
			DrawUtils::drawNameTags(ent, 0.95f, NameTagsMod->displayHealth, NameTagsMod->useUnicodeFont);
	}
}

void NameTags::onPostRender() {

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {

		g_Data.forEachEntity(drawNameTags);
	}
}