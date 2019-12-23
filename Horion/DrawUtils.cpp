#include "DrawUtils.h"


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

static __int64* tess_end_base = 0x0;

using tess_vertex_t = void(__fastcall*)(__int64 _this, float v1, float v2, float v3);
using tess_end_t = void(__fastcall*)(__int64, __int64 tesselator,__int64*);
using  mce__VertexFormat__disableHalfFloats_t = void(__fastcall*)(__int64, int,int);
using Tessellator__initializeFormat_t = void(__fastcall*)(__int64, __int64);

tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(Utils::FindSignature("48 8B C4 48 89 78 ?? 55 48 8D 68"));
tess_end_t tess_end = reinterpret_cast<tess_end_t>(Utils::FindSignature("40 53 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 49 8B F0 48 8B DA 48 8B F9"));
mce__VertexFormat__disableHalfFloats_t mce__VertexFormat__disableHalfFloats = reinterpret_cast<mce__VertexFormat__disableHalfFloats_t>(Utils::FindSignature("48 83 EC 28 4C 8B C9 C7 81 ?? ?? ?? ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81 ?? ?? ?? ?? ?? C6 81"));
Tessellator__initializeFormat_t Tessellator__initializeFormat = reinterpret_cast<Tessellator__initializeFormat_t>(Utils::FindSignature("48 89 74 24 ?? 57 48 83 EC 20 4C 8B 41 ?? 48 8B FA 4C 2B 41 ?? 48 8B F1 48 83 C1 08 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 49 F7 E8 48 D1 FA 48 8B C2 48 C1 E8 3F 48 03 D0 48 3B FA"));

void DrawUtils::tess__begin(__int64 tesselator) {
	if (!*(BYTE*)(tesselator + 0x1FC) && !*(BYTE*)(tesselator + 0x1B5)) {
		mce__VertexFormat__disableHalfFloats(tesselator,0,0); //guessed with tess_begin in 1.12
		*(BYTE*)(tesselator + 8) = 3;
		*(BYTE*)(tesselator + 0x1B4) = 0;
		*(WORD*)(tesselator + 0x1FC) = 1;
		*(DWORD*)(tesselator + 0x16C) = 0;
		*(__int64*)(tesselator + 0x150) = *(__int64*)(tesselator + 0x148);
		if (!*(BYTE*)tesselator)
			*(BYTE*)(tesselator + 0xD0) = 1;
		Tessellator__initializeFormat(tesselator + 8, 0x66i64);//same
	}
}


void DrawUtils::setCtx(C_MinecraftUIRenderContext * ctx, C_GuiData* gui) {
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
	if(g_Data.getClientInstance()->levelRenderer != nullptr)	
		origin = g_Data.getClientInstance()->levelRenderer->origin;

	if (tess_end_base == 0x0) {
		// 2 Sigs, wanted one comes first
		uintptr_t sigOffset = Utils::FindSignature("4C 8D 05 ?? ?? ?? ?? 48 8B D3 48 8B CF 48 8B 5C 24 ?? 0F 28 7C 24 ?? 44 0F 28 44 24 ?? 48");
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
	switch (font) {
	case Fonts::SMOOTH:
		return g_Data.getClientInstance()->N0000080D->getSmoothFont();
		break;
	case Fonts::UNICOD:
		return g_Data.getClientInstance()->_getUnicodeFont();
		break;
	case Fonts::RUNE:
		return g_Data.getClientInstance()->_getRuneFont();
		break;
	default:
		return g_Data.getClientInstance()->_getFont();
		break;
	}
}

float DrawUtils::getTextWidth(std::string * textStr, float textSize, Fonts font) {
	TextHolder* text = new TextHolder(*textStr);

	C_Font* fontPtr = getFont(font);

	float ret = renderCtx->getLineLength(fontPtr, text, textSize, false);

	delete text;
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

	tess_end(a2, tesselator,tess_end_base);
}

void DrawUtils::fillRectangle(vec4_t pos, const MC_Color col, float alpha) {
	float* posF = new float[4]; // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.z;
	posF[2] = pos.y;
	posF[3] = pos.w;

	MC_Color* c = new MC_Color(col);
	
	renderCtx->fillRectangle(posF, reinterpret_cast<float*>(c), alpha);
	
	delete c;
	delete[] posF;
}

void DrawUtils::drawRectangle(vec4_t pos, MC_Color col, float alpha, float lineWidth) {
	lineWidth /= 2;
	fillRectangle(vec4_t(pos.x - lineWidth, pos.y - lineWidth, pos.z + lineWidth, pos.y + lineWidth), col, alpha); // TOP
	fillRectangle(vec4_t(pos.x - lineWidth, pos.y            , pos.x + lineWidth, pos.w            ), col, alpha); // LEFT
	fillRectangle(vec4_t(pos.z - lineWidth, pos.y            , pos.z + lineWidth, pos.w            ), col, alpha); // 
	fillRectangle(vec4_t(pos.x - lineWidth, pos.w - lineWidth, pos.z + lineWidth, pos.w + lineWidth), col, alpha);
}

void DrawUtils::drawText(vec2_t pos, std::string* textStr, MC_Color *color, float textSize, Fonts font) {
	static MC_Color* WHITE_COLOR = new MC_Color(1, 1, 1, 1, false);
	if (color == nullptr)
		color = WHITE_COLOR;
	
	TextHolder* text = new TextHolder(*textStr);
	C_Font* fontPtr = getFont(font);
	static uintptr_t caretMeasureData = 0xFFFFFFFF;

	pos.y -= 1;

	float* posF = new float[4]; // vec4_t(startX, startY, endX, endY);
	posF[0] = pos.x;
	posF[1] = pos.x + 1000;
	posF[2] = pos.y;
	posF[3] = pos.y + 1000;

	static float size = 1;
	size = textSize;
	renderCtx->drawText(fontPtr, posF, text, color->arr, 1, 0, &size, &caretMeasureData);

	if (color->shouldDelete)
		delete color;
	delete[] posF;
	delete text;
	
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

	for (int i = 0; i < 24; i += 2)
		if (refdef->OWorldToScreen(origin, cornerList[i], Screen1, fov, screenSize) && refdef->OWorldToScreen(origin, cornerList[i + 1], Screen2, fov, screenSize)) {
			drawLine(Screen1, Screen2, lineWidth);
		}
	
	static Tracer* mod = moduleMgr->getModule<Tracer>();
	if (mod == nullptr)
		mod = moduleMgr->getModule<Tracer>();
	else if (mod->isEnabled()) {
		// REWORK ASAP
		vec2_t renderEnable(((g_Data.getClientInstance()->getGuiData()->widthGame) / 2), ((g_Data.getClientInstance()->getGuiData()->heightGame) / 2));
		if(Screen2.y > 0)
			DrawUtils::drawLine(renderEnable, Screen2, lineWidth);
	}
}

void DrawUtils::drawNameTags(C_Entity* ent, float textSize, bool drawHealth, bool useUnicodeFont) {
	vec2_t textPos;
	std::string text = ent->getNameTag()->getText();

	float textStr = getTextWidth(&text);

	if (refdef->OWorldToScreen(origin,ent->eyePos0, textPos, fov, screenSize)) {
		textPos.y -= 10.f;
		textPos.x -= textStr / 2.f;
		drawText(textPos, &text, nullptr, textSize);
	}
}


void DrawUtils::rainbow(float* rcolors) {
	if (rcolors[3] < 1) {
		rcolors[0] = 1;
		rcolors[1] = 0.6f;
		rcolors[2] = 0.6f;
		rcolors[3] = 1;
	}

	Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]); // perfect code, dont question this

	rcolors[0] += 0.003f;
	if (rcolors[0] >= 1)
		rcolors[0] = 0;

	Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
}
void DrawUtils::drawEntityBox(C_Entity * ent, float lineWidth) {
	vec3_t* start = ent->getPosOld();
	vec3_t* end = ent->getPos();
	
	vec3_t lerped = start->lerp(end, lerpT);

	AABB render(lerped, ent->width, ent->height, end->y - ent->aabb.lower.y);
	render.upper.y += 0.1f;

	drawBox(render.lower, render.upper, lineWidth);
}

void DrawUtils::wirebox(AABB aabb){
	/*
	using tesselatorWirebox_t = void(_fastcall*)(__int64 _this, AABB aabb);
	static tesselatorWirebox_t tesselateWirebox = reinterpret_cast<tesselatorWirebox_t>(Utils::FindSignature("48 89 5C 24 ?? 57 48 83 EC ?? 48 8B DA 48 C7 44 24 ?? 00 00 00 00 B2 04"));
	
	float* v15 = reinterpret_cast<float*>(tesselator);
	float* yote = reinterpret_cast<float*>(0x0000019367E06228 + 0x448);
	v15[41] = -yote[0];
	v15[42] = -yote[1];
	v15[43] = -yote[2];
	setColor(1, 1, 0, 1);

	tesselateWirebox(tesselator, aabb);
	tess_end(0x0000019354E40E08, tesselator, reinterpret_cast<__int64*>(0x0000019367E07288));

	v15[41] = 0;
	v15[42] = 0;
	v15[43] = 0;*/
	throw std::exception("not implemented");
}

vec2_t DrawUtils::worldToScreen(vec3_t world) {
	vec2_t ret;
	bool success = refdef->OWorldToScreen(origin, world, ret, fov, screenSize);
	return ret;
}
