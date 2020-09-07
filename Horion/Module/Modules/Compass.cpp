#include "Compass.h"

Compass::Compass() : IModule(0x0, Category::VISUAL, "Compass") {
	registerFloatSetting("Opacity", &opacity, opacity, 0.1, 1);
	registerIntSetting("Range", &range, range, 45, 180);
}

Compass::~Compass() {
}

const char* Compass::getModuleName() {
	return "Compass";
}

void Compass::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	if (player == nullptr || !GameData::canUseMoveKeys()) return;

	int deg = player->yaw + 180;
	float sCenter = g_Data.getGuiData()->widthGame / 2;

	for (int off = -range; off <= range; off++) {
		int oDeg = (deg + off) % 360;
		if (oDeg < 0) oDeg += 360;
		switch (oDeg) {
		case 0:
			drawCenteredText(vec2_t(sCenter + off, 30), "N", 1);
			break;
		case 45:
			drawCenteredText(vec2_t(sCenter + off, 30), "NE", 1);
			break;
		case 90:
			drawCenteredText(vec2_t(sCenter + off, 30), "E", 1);
			break;
		case 135:
			drawCenteredText(vec2_t(sCenter + off, 30), "SE", 1);
			break;
		case 180:
			drawCenteredText(vec2_t(sCenter + off, 30), "S", 1);
			break;
		case 225:
			drawCenteredText(vec2_t(sCenter + off, 30), "SW", 1);
			break;
		case 270:
			drawCenteredText(vec2_t(sCenter + off, 30), "W", 1);
			break;
		case 315:
			drawCenteredText(vec2_t(sCenter + off, 30), "NW", 1);
			break;
		}
		if (off != 0) {
			if ((oDeg % 15) != 0) {
				if ((oDeg % 5) == 0) {
					DrawUtils::fillRectangle(vec4_t(sCenter + off - 0.25, 16, sCenter + off + 0.25, 19), MC_Color(255, 255, 255), opacity);
				}
				continue;
			}
			if (off <= 10 && off >= -10) {
				DrawUtils::fillRectangle(vec4_t(sCenter + off - 0.25, 16, sCenter + off + 0.25, 19), MC_Color(255, 255, 255), opacity);
				continue;
			}
			DrawUtils::fillRectangle(vec4_t(sCenter + off - 0.5, 15, sCenter + off + 0.5, 20), MC_Color(255, 255, 255), opacity);
			drawCenteredText(vec2_t(sCenter + off, 20), std::to_string(oDeg), 0.75);
		}
	}
	DrawUtils::fillRectangle(vec4_t(sCenter - 0.5, 15, sCenter + 0.5, 25), MC_Color(255, 255, 255), opacity);
	drawCenteredText(vec2_t(sCenter, 25), std::to_string(deg), 0.75);
	DrawUtils::flush();
}

void Compass::drawCenteredText(vec2_t pos, std::string text, float size) {
	pos.x -= DrawUtils::getTextWidth(&text, size) / 2;
	DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, opacity);
}
