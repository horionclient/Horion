#include "Compass.h"

Compass::Compass() : IModule(0x0, Category::VISUAL, "Compass") {
	registerFloatSetting("Opacity", &opacity, opacity, 0.1f, 1);
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

	const int deg = (int)(player->yaw + 180);
	const float degSubOffset = 0; // -fmodf(player->yaw, 1)
	const float sCenter = g_Data.getGuiData()->widthGame / 2;

	for (int off = -range; off <= range; off++) {
		int oDeg = (deg + off) % 360;
		if (oDeg < 0) oDeg += 360;

		const float xOff = sCenter + off + degSubOffset;
		constexpr float fadeOutPos = 0.25f;
		constexpr float centerCutoff = 5, centerFadeout = 10;
		float majorOpacity = this->opacity;  // big compass text
		float minorOpacity = majorOpacity;   // minor features, degree numbers
		
		// Fading logic
		{
			if((range - abs(off)) < range * fadeOutPos) // Far from center 
				minorOpacity = majorOpacity = lerp(0, opacity, (range - abs(off)) / (range * fadeOutPos));
			else if (abs(off) < centerFadeout)  // Close to center
				minorOpacity = lerp(0, opacity, (abs(off) - centerCutoff) / (centerFadeout - centerCutoff));
		}

		switch (oDeg) {
		case 0:
			drawCenteredText(vec2_t(xOff, 30), "N", 1, majorOpacity);
			break;
		case 45:
			drawCenteredText(vec2_t(xOff, 30), "NE", 1, majorOpacity);
			break;
		case 90:
			drawCenteredText(vec2_t(xOff, 30), "E", 1, majorOpacity);
			break;
		case 135:
			drawCenteredText(vec2_t(xOff, 30), "SE", 1, majorOpacity);
			break;
		case 180:
			drawCenteredText(vec2_t(xOff, 30), "S", 1, majorOpacity);
			break;
		case 225:
			drawCenteredText(vec2_t(xOff, 30), "SW", 1, majorOpacity);
			break;
		case 270:
			drawCenteredText(vec2_t(xOff, 30), "W", 1, majorOpacity);
			break;
		case 315:
			drawCenteredText(vec2_t(xOff, 30), "NW", 1, majorOpacity);
			break;
		}
		if (off != 0 && minorOpacity > 0) {
			if ((oDeg % 15) != 0) {
				if ((oDeg % 5) == 0) {
					DrawUtils::fillRectangle(vec4_t(xOff - 0.25f, 16, xOff + 0.25f, 19), MC_Color(255, 255, 255), minorOpacity);
				}
				continue;
			}

			// Bigger line with degree displayed
			DrawUtils::fillRectangle(vec4_t(xOff - 0.5f, 15, xOff + 0.5f, 20), MC_Color(255, 255, 255), minorOpacity);
			drawCenteredText(vec2_t(xOff, 20), std::to_string(oDeg), 0.75f, minorOpacity);
		}
	}

	// Center line
	DrawUtils::fillRectangle(vec4_t(sCenter - 0.5f, 15, sCenter + 0.5f, 25), MC_Color(255, 255, 255), opacity);
	drawCenteredText(vec2_t(sCenter, 25), std::to_string(deg), 0.75f, opacity);
	DrawUtils::flush();
}

void Compass::drawCenteredText(vec2_t pos, std::string text, float size, float textOpacity) {
	pos.x -= DrawUtils::getTextWidth(&text, size) / 2;
	DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, textOpacity);
}
