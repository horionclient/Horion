#include "HudModule.h"
#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"

HudModule::HudModule() : IModule(0, Category::VISUAL, "Displays ArrayList/TabGUI") {
	registerBoolSetting("TabGui", &this->tabgui, this->tabgui);
	registerBoolSetting("ArrayList", &this->arraylist, this->arraylist);
	registerBoolSetting("ClickToggle", &this->clickToggle, this->clickToggle);
	registerBoolSetting("Watermark", &this->watermark, this->watermark);
	registerBoolSetting("Coordinates", &this->coordinates, this->coordinates);
	registerBoolSetting("Show Keybinds", &this->keybinds, this->keybinds);
	registerBoolSetting("Show ArmorHUD", &this->displayArmor, this->displayArmor);
	registerBoolSetting("Keystrokes", &this->keystrokes, this->keystrokes);
	registerBoolSetting("Show FPS", &this->fps, this->fps);
	registerBoolSetting("Show CPS", &this->cps, this->cps);
	registerBoolSetting("Always show", &this->alwaysShow, this->alwaysShow);
	registerFloatSetting("Scale", &this->scale, this->scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::drawKeystroke(char key, const vec2_t& pos) {
	std::string keyString = Utils::getKeybindName(key);
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (key == *input->forwardKey) {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 20.f,
			pos.y + 20.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f) + 0.2,
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
	if (key == *input->spaceBarKey) {
		if (key == *input->spaceBarKey) keyString = "-";
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 64.f,
			pos.y + 15.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	} else {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f),
			pos.y + ((key == *input->spaceBarKey) ? 15.f : 20.f));
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
}

void HudModule::drawLeftMouseKeystroke(vec2_t pos) {
	std::string keyString;
	keyString = "LMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRectangle(rectPos, GameData::GameData::isLeftClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::drawRightMouseKeystroke(vec2_t pos) {
	std::string keyString;
	keyString = "RMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRectangle(rectPos, GameData::GameData::isRightClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * this->scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	if(tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;
	{  // FPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->fps)) {
			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 1.f);
			DrawUtils::drawText(textPos, &fpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}
	}
	{  // CPS
		if (!(g_Data.getLocalPlayer() == nullptr || !this->cps)) {
			std::string cpsText = "CPS: " + std::to_string(g_Data.getLeftCPS()) + " - " + std::to_string(g_Data.getRightCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 1.f);
			DrawUtils::drawText(textPos, &cpsText, MC_Color(200, 200, 200), scale);

			startY += f;
		}
	}
	{  // Coordinates
		if (!(g_Data.getLocalPlayer() == nullptr || !this->coordinates)) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();

			std::string coordsX = "X: " + std::to_string((int)floorf(pos->x));
			std::string coordsY = "Y: " + std::to_string((int)floorf(pos->y));
			std::string coordsZ = "Z: " + std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 1.f);
			DrawUtils::fillRectangle(rectPos, MC_Color(12, 12, 12), 1.f);
			DrawUtils::drawText(textPos, &coordsX, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsY, MC_Color(200, 200, 200), scale);
			textPos.y += f;
			DrawUtils::drawText(textPos, &coordsZ, MC_Color(200, 200, 200), scale);
		}
	}
	{  // ArmorHUD
		
		if (!(g_Data.getLocalPlayer() == nullptr || !this->displayArmor || !GameData::canUseMoveKeys())) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();
			float x = windowSize.x / 2.f + 5.f;
			float y = windowSize.y - 57.5f;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					x += scale * spacing;
				}
			}
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			//x += scale * spacing;
			if (item->isValid())
				DrawUtils::drawItem(item, vec2_t(x, y), opacity, scale, item->isEnchanted());
		}
	}
	{  // Keystrokes
		if (!(g_Data.getLocalPlayer() == nullptr || !this->keystrokes || !GameData::canUseMoveKeys())) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			HudModule::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 84));
			HudModule::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 40));
			HudModule::drawLeftMouseKeystroke(vec2_t(10.f, windowSize.y - 25));
			HudModule::drawRightMouseKeystroke(vec2_t(43.f, windowSize.y - 25));
		}
	}
}