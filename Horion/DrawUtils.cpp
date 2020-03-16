#include "DrawUtils.h"

#include "Module/ModuleManager.h"

C_MinecraftUIRenderContext* renderCtx;
C_GuiData* guiData;
__int64 a2;
__int64 tesselator;
float* colorHolder;
glmatrixf* refdef;
vec2_t fov;
vec2_t screenSize;
vec3_t origin;
float lerpT;
C_TexturePtr* texturePtr = nullptr;

static __int64* tess_end_base = 0x0;


tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("48 8B C4 48 89 78 ?? 55 48 8D 68"));
tess_end_t tess_end = reinterpret_cast<tess_end_t>(FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 49 8B F0 48 8B DA 48 8B F9"));
mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats = reinterpret_cast<mce__VertexFormat__disableHalfFloats_t>(FindSignature("48 83 EC 28 4C 8B C9 C7 81 ?? ?? ?? ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81"));
Tessellator__initializeFormat_t Tessellator__initializeFormat = reinterpret_cast<Tessellator__initializeFormat_t>(FindSignature("48 89 74 24 ?? 57 48 83 EC 20 4C 8B 41 ?? 48 8B FA 4C 2B 41 ?? 48 8B F1 48 83 C1 08 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 F7 E8 48 D1 FA 48 8B C2 48 C1 E8 3F 48 03 D0 48 3B FA"));

void DrawUtils::setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* gui) {
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime().QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / 20;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	a2 = reinterpret_cast<__int64*>(renderCtx)[2];
	tesselator = *reinterpret_cast<__int64*>(a2 + 0xA8);
	colorHolder = *reinterpret_cast<float**>(a2 + 0x30);

	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();
	refdef = badrefdef->correct();
	fov = g_Data.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->origin;

	if (tess_end_base == 0x0) {
		// 2 Sigs, wanted one comes first
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		tess_end_base = reinterpret_cast<__int64*>(sigOffset + offset + 7);
	}
}

void DrawUtils::setColor(float r, float g, float b, float a) {
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

C_Font* DrawUtils::getFont(Fonts font) {
	static bool check = strcmp(GameData::getGameVersion()->getText(), "1.14.0");
	if (check == 0)
		return g_Data.getClientInstance()->N0000080D->getOldFont();
	switch (font) {
	case Fonts::SMOOTH:
		return g_Data.getClientInstance()->N0000080D->getTheGoodFontThankYou();
		break;
	case Fonts::UNICOD:
		return g_Data.getClientInstance()->N0000080D->getTheBetterFontYes();
		break;
	case Fonts::RUNE:
		return g_Data.getClientInstance()->_getRuneFont();
		break;
	default:
		return g_Data.getClientInstance()->_getFont();
		break;
	}
}

float DrawUtils::getTextWidth(std::string* textStr, float textSize, Fonts font) {
	TextHolder text(*textStr);

	C_Font* fontPtr = getFont(font);

	float ret = renderCtx->getLineLength(fontPtr, &text, textSize, false);

	return ret;
}

void DrawUtils::flush() {
	renderCtx->flushText(0);
}

void DrawUtils::drawLine(vec2_t start, vec2_t end, float lineWidth) {
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	DrawUtils::tess__begin(tesselator);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_end(a2, tesselator, tess_end_base);
}

void DrawUtils::drawText(vec2_t pos, std::string* textStr, MC_Color color, float textSize, float alpha, Fonts font) {
	TextHolder text(*textStr);
	C_Font* fontPtr = getFont(font);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;

	pos.y -= 1;

	float posF[4];  // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y;
	posF[3] = pos.y + 1000;

	TextMeasureData textMeasure;
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;
	
	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}

void DrawUtils::drawBox(vec3_t lower, vec3_t upper, float lineWidth) {
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t cornerList[24];
	cornerList[0] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);

	cornerList[2] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[3] = vec3_t(lower.x, lower.y, lower.z + diff.z);

	cornerList[4] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	cornerList[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);

	cornerList[6] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	cornerList[7] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);

	cornerList[8] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[9] = vec3_t(lower.x, lower.y + diff.y, lower.z);

	cornerList[10] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	cornerList[11] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);

	cornerList[12] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	cornerList[13] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[14] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	cornerList[15] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[16] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	cornerList[17] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);

	cornerList[18] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	cornerList[19] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[20] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	cornerList[21] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[22] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	cornerList[23] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	vec2_t Screen1;
	vec2_t Screen2;

	for (int i = 0; i < 24; i += 2) {
		if (refdef->OWorldToScreen(origin, cornerList[i], Screen1, fov, screenSize) && refdef->OWorldToScreen(origin, cornerList[i + 1], Screen2, fov, screenSize)) {
			drawLine(Screen1, Screen2, lineWidth);
		}
	}
}

void DrawUtils::drawTracer(C_Entity* ent) {
	vec2_t target;
	refdef->OWorldToScreen(origin, *ent->getPos(), target, fov, screenSize);
	vec2_t mid(((g_Data.getClientInstance()->getGuiData()->widthGame) / 2), ((g_Data.getClientInstance()->getGuiData()->heightGame) / 2));
	if(target != vec2_t(0, 0)) DrawUtils::drawLine(mid, target, 0.2f);
}

void DrawUtils::drawImage(std::string FilePath, vec2_t& imagePos, vec2_t& ImageDimension, vec2_t& idk) {
	if (texturePtr == nullptr) {
		texturePtr = new C_TexturePtr();
		C_FilePath file(FilePath);
		renderCtx->getTexture(texturePtr, file);
	}

	__int64 yot = 0;
	static __int64 hashedString = 0xA99285D21E94FC80;
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 0F 29 70 ?? 0F 29 78 ?? 44 0F 29 40 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 4D 8B E1 44 0F 28 C2 4C 8B F2 4C 8B F9");

	if (texturePtr != nullptr) {
		renderCtx->drawImage(texturePtr, imagePos, ImageDimension, yot, idk);
		renderCtx->flushImages(MC_Color(1.f, 1.f, 1.f), flushImageAddr, (__int64)&hashedString);
	}
}

void DrawUtils::drawNameTags(C_Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	vec2_t textPos;
	vec4_t rectPos;
	std::string text = ent->getNameTag()->getText();
	text = Utils::sanitize(text);

	float textStr = getTextWidth(&text);

	if (refdef->OWorldToScreen(origin, ent->eyePos0, textPos, fov, screenSize)) {
		textPos.y -= 10.f;
		textPos.x -= textStr / 2.f;
		rectPos.x = textPos.x - 1.f;
		rectPos.y = textPos.y - 1.f;
		rectPos.z = textPos.x + textStr + 25.f;
		rectPos.w = textPos.y + 10.f;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f;
		fillRectangle(rectPos, MC_Color(13, 29, 48), 0.5f);
		fillRectangle(subRectPos, MC_Color(28, 107, 201), 0.5f);
		drawText(textPos, &text, MC_Color(255, 255, 255), textSize);

		if (ent->getEntityTypeId() == 63 && moduleMgr->getModule<NameTags>()->displayArmor) {  // is player, show armor
			C_Player* player = static_cast<C_Player*>(ent);
			static float constexpr scale = 0.4f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 15.f;
			float x = rectPos.z - 25.f;
			float y = rectPos.w - 10.f;
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					x += scale * spacing;
				}
			}
		}
	}
}

void DrawUtils::drawEntityBox(C_Entity* ent, float lineWidth) {
	vec3_t* start = ent->getPosOld();
	vec3_t* end = ent->getPos();

	vec3_t lerped = start->lerp(end, lerpT);

	AABB render(lerped, ent->width, ent->height, end->y - ent->aabb.lower.y);
	render.upper.y += 0.1f;

	drawBox(render.lower, render.upper, lineWidth);
}

void DrawUtils::draw2D(C_Entity* ent, float lineWidth) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

	vec3_t upper = ent->getAABB()->upper;
	vec3_t lower = ent->getAABB()->lower;
	
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t cornerList[24];
	cornerList[0] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);

	cornerList[2] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[3] = vec3_t(lower.x, lower.y, lower.z + diff.z);

	cornerList[4] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	cornerList[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);

	cornerList[6] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	cornerList[7] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);

	cornerList[8] = vec3_t(lower.x, lower.y, lower.z);
	cornerList[9] = vec3_t(lower.x, lower.y + diff.y, lower.z);

	cornerList[10] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	cornerList[11] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);

	cornerList[12] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	cornerList[13] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[14] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	cornerList[15] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[16] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	cornerList[17] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);

	cornerList[18] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	cornerList[19] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[20] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	cornerList[21] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	cornerList[22] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	cornerList[23] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	vec2_t closestCorners[4];
	for (int n = 0; n < 4; n++) {
		double dist = INFINITE;
		for (int i = 0; i < 24; i++) {
			vec2_t corner;
			if (refdef->OWorldToScreen(origin, cornerList[i], corner, fov, screenSize)) {
				double yot;
				switch (n) {
					case 0:
						yot = sqrt(pow(0 - corner.x, 2) + pow(0 - corner.y, 2) * 1.0);
						break;
					case 1:
						yot = sqrt(pow(windowSize.x - corner.x, 2) + pow(0 - corner.y, 2) * 1.0);
						break;
					case 2:
						yot = sqrt(pow(0 - corner.x, 2) + pow(windowSize.y - corner.y, 2) * 1.0);
						break;
					case 3:
						yot = sqrt(pow(windowSize.x - corner.x, 2) + pow(windowSize.y - corner.y, 2) * 1.0);
						break;
				}
				if (yot < dist) {
					dist = yot;
					closestCorners[n] = corner;
				}
			}
		}
	}
	drawRectangle(vec4_t(closestCorners[0].x, closestCorners[0].y, closestCorners[3].x, closestCorners[3].y), colorHolder, 1.f, lineWidth);
}

void DrawUtils::drawItem(C_ItemStack* item, vec2_t ItemPos, float opacity, float scale, bool isEnchanted) {
	__int64 ScnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	C_ScreenContext* ScreenCtx = reinterpret_cast<C_ScreenContext*>(ScnCtx);
	C_BaseActorRenderContext baseActorRenderCtx(ScreenCtx, g_Data.getClientInstance(), g_Data.getClientInstance()->minecraftGame);
	C_ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, g_Data.getClientInstance()->minecraftGame, ItemPos.x, ItemPos.y, opacity, scale, isEnchanted);
}

void DrawUtils::drawKeystroke(char key, vec2_t pos) {
	std::string keyString = Utils::getKeybindName(key);
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (key == *input->spaceBarKey) keyString = "-";
	vec4_t rectPos(
		pos.x,
		pos.y,
		pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f),
		pos.y + 20.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(28, 50, 77) : MC_Color(13, 29, 48), 1.f);
	drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

vec2_t DrawUtils::worldToScreen(vec3_t world) {
	vec2_t ret;
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
