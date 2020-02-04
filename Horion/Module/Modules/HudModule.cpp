#include "HudModule.h"

HudModule::HudModule() : IModule(0x0, Category::VISUAL, "Displays ArrayList/TabGUI") {
	registerBoolSetting("TabGui", &this->tabgui, this->tabgui);
	registerBoolSetting("ArrayList", &this->arraylist, this->arraylist);
	registerBoolSetting("ClickToggle", &this->clickToggle, this->clickToggle);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("Coordinates", &this->coordinates, this->coordinates);
	registerBoolSetting("Show Keybinds", &this->keybinds, this->keybinds);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !this->displayArmor || !GameData::canUseMoveKeys())
		return;
	static float constexpr scale = 1.f;
	static float constexpr opacity = 0.25f;
	static float constexpr spacing = scale + 15.f;
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	float x = g_Data.getClientInstance()->getGuiData()->windowSize.x / 2.f + 5.f;
	float y = g_Data.getClientInstance()->getGuiData()->windowSize.y - 57.5f;
	for (int i = 0; i < 4; i++) {
		if (player->getArmor(i)->item != nullptr) {
			DrawUtils::drawItem(player->getArmor(i),vec2_t(x,y),opacity, scale, player->getArmor(i)->isEnchanted());
			x += scale * spacing;
		}
	}
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
	//x += scale * spacing;
	if (item->item != nullptr)
		DrawUtils::drawItem(item, vec2_t(x, y), opacity, scale, item->isEnchanted());
}
