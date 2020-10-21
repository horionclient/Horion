#include "DrawUtils.h"

#include "Module/ModuleManager.h"
#include <Windows.h>
#include "../Utils/Logger.h"

struct MaterialPtr {
	char padding[0x138];
};

using tess_vertex_t = void(__fastcall*)(__int64 _this, float v1, float v2, float v3);
using tess_end_t = void(__fastcall*)(__int64, __int64 tesselator, MaterialPtr*);

C_MinecraftUIRenderContext* renderCtx;
C_GuiData* guiData;
__int64 screenContext2d;
__int64 game3dContext;
__int64 tesselator;
float* colorHolder;
std::shared_ptr<glmatrixf> refdef;
vec2_t fov;
vec2_t screenSize;
vec3_t origin;
float lerpT;
C_TexturePtr* texturePtr = nullptr;

static MaterialPtr* uiMaterial = nullptr;
static MaterialPtr* entityFlatStaticMaterial = nullptr;

tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(FindSignature("48 8B C4 48 89 78 ?? 55 48 8D 68"));
tess_end_t tess_end = reinterpret_cast<tess_end_t>(FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 49 8B F0 48 8B DA 48 8B F9"));
mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats = reinterpret_cast<mce__VertexFormat__disableHalfFloats_t>(FindSignature("48 83 EC 28 4C 8B C9 C7 81 ?? ?? ?? ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81"));
Tessellator__initializeFormat_t Tessellator__initializeFormat = reinterpret_cast<Tessellator__initializeFormat_t>(FindSignature("48 89 74 24 ?? 57 48 83 EC 20 4C 8B 41 ?? 48 8B FA 4C 2B 41 ?? 48 8B F1 48 83 C1 08 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 F7 E8 48 D1 FA 48 8B C2 48 C1 E8 3F 48 03 D0 48 3B FA"));

void DrawUtils::setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* gui) {
	LARGE_INTEGER EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

	ElapsedMicroseconds.QuadPart *= 1000000;
	int ticksPerSecond = 20;
	if(g_Data.getClientInstance()->minecraft)
		ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
	if(ticksPerSecond < 1)
		ticksPerSecond = 1;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
	lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
	if (lerpT > 1)
		lerpT = 1;
	else if (lerpT < 0)
		lerpT = 0;

	guiData = gui;
	renderCtx = ctx;
	screenContext2d = reinterpret_cast<__int64*>(renderCtx)[2];

	tesselator = *reinterpret_cast<__int64*>(screenContext2d + 0xB0);
	colorHolder = *reinterpret_cast<float**>(screenContext2d + 0x30);

	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();

	refdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	fov = g_Data.getClientInstance()->getFov();
	screenSize.x = gui->widthGame;
	screenSize.y = gui->heightGame;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->origin;

	if (uiMaterial == nullptr) {
		// 2 Sigs, wanted one comes first
		uintptr_t sigOffset = FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		uiMaterial = reinterpret_cast<MaterialPtr*>(sigOffset + offset + 7);
	}
	if(entityFlatStaticMaterial == nullptr && g_Data.isInGame()){
		entityFlatStaticMaterial = reinterpret_cast<MaterialPtr*>(g_Data.getClientInstance()->itemInHandRenderer->entityLineMaterial.materialPtr);
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

	if (true)
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

	DrawUtils::tess__begin(tesselator, 3);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_end(screenContext2d, tesselator, uiMaterial);
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

	TextMeasureData textMeasure{};
	memset(&textMeasure, 0, sizeof(TextMeasureData));
	textMeasure.textSize = textSize;

	renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &textMeasure, &caretMeasureData);
}

void DrawUtils::drawBox(vec3_t lower, vec3_t upper, float lineWidth, bool outline) {
	
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	
	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	// Convert to screen coord
	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++) {
		vec2_t screen;
		if (refdef->OWorldToScreen(origin, vertices[i], screen, fov, screenSize)) {
			screenCords.emplace_back(outline ? (int) screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return;  // No lines possible

	if (!outline) {
		for (auto it = screenCords.begin(); it != screenCords.end(); it++) {
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords) {
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x; 
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y; 
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z; 
				
				if (shouldDraw)
					drawLine(std::get<1>(from), std::get<1>(to), lineWidth);
			}
		}

		return;
	}
	// Find start vertex
	auto it = screenCords.begin();
	std::tuple<int, vec2_t> start = *it;
	it++;
	for (; it != screenCords.end(); it++) {
		auto cur = *it;
		if (std::get<1>(cur).x < std::get<1>(start).x) {
			start = cur;
		}
	}

	// Follow outer line
	std::vector<int> indices;

	auto current = start;
	indices.push_back(std::get<0>(current));
	vec2_t lastDir(0, -1);
	do {
		float smallestAngle = PI * 2;
		vec2_t smallestDir;
		std::tuple<int, vec2_t> smallestE;
		auto lastDirAtan2 = atan2(lastDir.y, lastDir.x);
		for (auto cur : screenCords) {
			if (std::get<0>(current) == std::get<0>(cur))
				continue;

			// angle between vecs
			vec2_t dir = vec2_t(std::get<1>(cur)).sub(std::get<1>(current));
			float angle = atan2(dir.y, dir.x) - lastDirAtan2;
			if (angle > PI) {
				angle -= 2 * PI;
			} else if (angle <= -PI) {
				angle += 2 * PI;
			}
			if (angle >= 0 && angle < smallestAngle) {
				smallestAngle = angle;
				smallestDir = dir;
				smallestE = cur;
			}
		}
		indices.push_back(std::get<0>(smallestE));
		lastDir = smallestDir;
		current = smallestE;
	} while (std::get<0>(current) != std::get<0>(start) && indices.size() < 8);

	// draw
	
	vec2_t lastVertex;
	bool hasLastVertex = false;
	for (auto& indice : indices) {
		vec2_t curVertex = std::get<1>(screenCords[indice]);
		if (!hasLastVertex) {
			hasLastVertex = true;
			lastVertex = curVertex;
			continue;
		}
		
		drawLine(lastVertex, curVertex, lineWidth);
		lastVertex = curVertex;
	}
}

void DrawUtils::drawImage(std::string FilePath, vec2_t& imagePos, vec2_t& ImageDimension, vec2_t& idk) {
	if (texturePtr == nullptr) {
		texturePtr = new C_TexturePtr();
		C_FilePath file(FilePath);
		renderCtx->getTexture(texturePtr, file);
	}

	__int64 yot = 0;
	static unsigned __int64 hashedString = 0xA99285D21E94FC80;
	static uintptr_t flushImageAddr = FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 0F 29 70 ?? 0F 29 78 ?? 44 0F 29 40 ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 4D 8B E1 44 0F 28 C2 4C 8B F2 4C 8B F9");

	if (texturePtr != nullptr) {
		renderCtx->drawImage(texturePtr, imagePos, ImageDimension, yot, idk);
		MC_Color col(1.f, 1.f, 1.f);
		renderCtx->flushImages(col, flushImageAddr, (__int64)&hashedString);
	}
}

void DrawUtils::drawNameTags(C_Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	vec2_t textPos;
	vec4_t rectPos;
	std::string text = ent->getNameTag()->getText();
	text = Utils::sanitize(text);

	float textWidth = getTextWidth(&text, textSize);
	float textHeight = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * textSize;

	if (refdef->OWorldToScreen(origin, ent->eyePos0.add(0, 0.5f, 0), textPos, fov, screenSize)) {
		textPos.y -= textHeight;
		textPos.x -= textWidth / 2.f;
		rectPos.x = textPos.x - 1.f * textSize;
		rectPos.y = textPos.y - 1.f * textSize;
		rectPos.z = textPos.x + textWidth + 1.f * textSize;
		rectPos.w = textPos.y + textHeight + 2.f * textSize;
		vec4_t subRectPos = rectPos;
		subRectPos.y = subRectPos.w - 1.f * textSize;
		fillRectangle(rectPos, MC_Color(13, 29, 48), 0.8f);
		fillRectangle(subRectPos, MC_Color(28, 107, 201), 0.9f);
		drawText(textPos, &text, MC_Color(255, 255, 255), textSize);

		static auto nameTagsMod = moduleMgr->getModule<NameTags>();

		if (ent->getEntityTypeId() == 63 && nameTagsMod->displayArmor) {  // is player, show armor
			auto* player = reinterpret_cast<C_Player*>(ent);
			float scale = textSize * 0.6f;
			float spacing = scale + 15.f;
			float x = rectPos.x + 1.f * textSize;
			float y = rectPos.y - 20.f * scale;
			// armor
			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(x, y), 1.f, scale, stack->isEnchanted());
					x += scale * spacing;
				}
			}
			// item
			{
				C_ItemStack* stack = player->getSelectedItem();
				if (stack->item != nullptr) {
					DrawUtils::drawItem(stack, vec2_t(rectPos.z - 1.f - 15.f * scale, y), 1.f, scale, stack->isEnchanted());
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

	drawBox(render.lower, render.upper, lineWidth, true);
}

void DrawUtils::draw2D(C_Entity* ent, float lineWidth) {
	vec3_t base = vec3_t(ent->eyePos0.x, ent->eyePos0.y + 0.15f, ent->eyePos0.z);
	float ofs = (g_Data.getLocalPlayer()->yaw + 90.f) * (PI / 180);

	vec3_t corners[4];
	vec2_t corners2d[4];

	corners[0] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z - ent->width / 1.5f * cos(ofs));
	corners[1] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.upper.y + (float)0.1, base.z + ent->width / 1.5f * cos(ofs));
	corners[2] = vec3_t(base.x - ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z - ent->width / 1.5f * cos(ofs));
	corners[3] = vec3_t(base.x + ent->width / 1.5f * -sin(ofs), ent->aabb.lower.y, base.z + ent->width / 1.5f * cos(ofs));

	if (refdef->OWorldToScreen(origin, corners[0], corners2d[0], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[1], corners2d[1], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[2], corners2d[2], fov, screenSize) &&
		refdef->OWorldToScreen(origin, corners[3], corners2d[3], fov, screenSize)) {
		float length = (corners2d[1].x - corners2d[0].x) / 4.f;

		drawLine(corners2d[0], vec2_t(corners2d[0].x + length, corners2d[0].y), lineWidth);
		drawLine(corners2d[0], vec2_t(corners2d[0].x, corners2d[0].y + length), lineWidth);

		drawLine(vec2_t(corners2d[1].x - length, corners2d[1].y), corners2d[1], lineWidth);
		drawLine(corners2d[1], vec2_t(corners2d[1].x, corners2d[1].y + length), lineWidth);

		drawLine(vec2_t(corners2d[2].x, corners2d[2].y - length), corners2d[2], lineWidth);
		drawLine(corners2d[2], vec2_t(corners2d[2].x + length, corners2d[2].y), lineWidth);

		drawLine(vec2_t(corners2d[3].x, corners2d[3].y - length), corners2d[3], lineWidth);
		drawLine(vec2_t(corners2d[3].x - length, corners2d[3].y), corners2d[3], lineWidth);	

		/*drawLine(corners2d[0], corners2d[1], lineWidth);
		drawLine(corners2d[0], corners2d[2], lineWidth);
		drawLine(corners2d[3], corners2d[1], lineWidth);
		drawLine(corners2d[3], corners2d[2], lineWidth);*/
	}
}

void DrawUtils::drawItem(C_ItemStack* item, vec2_t itemPos, float opacity, float scale, bool isEnchanted) {
	__int64 scnCtx = reinterpret_cast<__int64*>(renderCtx)[2];
	auto* screenCtx = reinterpret_cast<C_ScreenContext*>(scnCtx);
	C_BaseActorRenderContext baseActorRenderCtx(screenCtx, g_Data.getClientInstance(), g_Data.getClientInstance()->minecraftGame);
	C_ItemRenderer* renderer = baseActorRenderCtx.renderer;
	renderer->renderGuiItemNew(&baseActorRenderCtx, item, g_Data.getClientInstance()->minecraftGame, itemPos.x, itemPos.y, opacity, scale, isEnchanted);
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

vec2_t DrawUtils::worldToScreen(const vec3_t& world) {
	vec2_t ret{-1, -1};
	refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
void DrawUtils::drawLine3d(const vec3_t& start, const vec3_t& end) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = *reinterpret_cast<__int64*>(game3dContext + 0xB0);

	DrawUtils::tess__begin(myTess, 4);

	auto start1 = start.sub(origin);
	auto end1 = end.sub(origin);

	tess_vertex(myTess, start1.x, start1.y, start1.z);
	tess_vertex(myTess, end1.x, end1.y, end1.z);

	tess_end(game3dContext, myTess, entityFlatStaticMaterial);
}
void DrawUtils::tess__begin(__int64 tesselator, int vertexFormat) {
	if (!*(unsigned char*)(tesselator + 0x1FC) && !*(unsigned char*)(tesselator + 0x1B5)) {
		mce__VertexFormat__disableHalfFloats(tesselator, 0, 0);
		*(unsigned char*)(tesselator + 8) = vertexFormat;
		*(unsigned char*)(tesselator + 0x1B4) = 0;
		*(unsigned short*)(tesselator + 0x1FC) = 1;
		*(unsigned int*)(tesselator + 0x16C) = 0;
		*(__int64*)(tesselator + 0x150) = *(__int64*)(tesselator + 0x148);
		if (!*(unsigned char*)tesselator)
			*(unsigned char*)(tesselator + 0xD0) = 1;
		//Tessellator__initializeFormat(tesselator + 8, 0x66i64);
	}
}
void DrawUtils::setGameRenderContext(__int64 ctx) {
	game3dContext = ctx;
	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		origin = g_Data.getClientInstance()->levelRenderer->origin;

	if(ctx){
		LARGE_INTEGER EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;
		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&EndingTime);
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - g_Data.getLastUpdateTime();

		ElapsedMicroseconds.QuadPart *= 1000000;
		int ticksPerSecond = 20;
		if(g_Data.getClientInstance()->minecraft)
			ticksPerSecond = (int)*g_Data.getClientInstance()->minecraft->timer;
		if(ticksPerSecond < 1)
			ticksPerSecond = 1;
		ElapsedMicroseconds.QuadPart /= Frequency.QuadPart / ticksPerSecond;
		lerpT = (ElapsedMicroseconds.QuadPart / 1000000.f);
		if (lerpT > 1)
			lerpT = 1;
		else if (lerpT < 0)
			lerpT = 0;
	}
}
float DrawUtils::getLerpTime() {
	return lerpT;
}
void DrawUtils::drawLinestrip3d(const std::vector<vec3_t>& points) {
	if(game3dContext == 0 || entityFlatStaticMaterial == 0)
		return;

	auto myTess = *reinterpret_cast<__int64*>(game3dContext + 0xB0);

	DrawUtils::tess__begin(myTess, 5);

	/*
	 * 1: quads
	 * 2: triangle list
	 * 3: trianglestrip (6)
	 * 4: line list
	 * 5: line strip (7)
	 */

	for(const auto& p : points){
		auto pD = p.sub(origin);
		tess_vertex(myTess, pD.x, pD.y, pD.z);
	}

	tess_end(game3dContext, myTess, entityFlatStaticMaterial);
}
