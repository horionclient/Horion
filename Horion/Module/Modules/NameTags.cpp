#include "NameTags.h"

#include "../../../Utils/Target.h"
#include "../ModuleManager.h"

NameTags::NameTags() : IModule(0x0, Category::VISUAL, "Shows better nametags above players that can be seen from a lot more far aways") {
	this->registerBoolSetting("Display Armor", &this->displayArmor, this->displayArmor);
}

NameTags::~NameTags() {
}

const char* NameTags::getModuleName() {
	return ("NameTags");
}

void drawNameTags(C_Entity* ent, bool isRegularEntitie) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	static auto nameTagsMod = moduleMgr->getModule<NameTags>();

	if (ent != localPlayer) {
		if (ent->timeSinceDeath > 0)
			return;
		if (ent->getNameTag()->getTextLength() < 1)
			return;
		if (Target::isValidTarget(ent) && nameTagsMod != nullptr) {
			nameTagsMod->nameTags.insert(Utils::sanitize(ent->getNameTag()->getText()));
			float dist = ent->getPos()->dist(*g_Data.getLocalPlayer()->getPos());
			DrawUtils::drawNameTags(ent, fmax(0.6f, 3.f / dist));
			DrawUtils::flush();
		}
			
	}
}

void NameTags::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (nameTags.size() > 100)
		nameTags.clear();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		std::vector<C_Entity*> temp;
		for (int i = 0; i < g_Data.getEntityList()->getListSize(); i++)
			temp.push_back(g_Data.getEntityList()->get(i));
		std::sort(temp.begin(), temp.end(), [localPlayer](const C_Entity* lhs, const C_Entity* rhs) {
			return localPlayer->getPos()->dist(*lhs->getPos()) > localPlayer->getPos()->dist(*rhs->getPos());
		});
		for (C_Entity* ent : temp)
			drawNameTags(ent, true);
	} else {
		nameTags.clear();
	}
}
