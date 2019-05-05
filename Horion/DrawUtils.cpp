#include "DrawUtils.h"

C_MinecraftUIRenderContext* renderCtx;
C_GuiData* guiData;
__int64 a2;
__int64 tesselator;
float* colorHolder;

static __int64* tess_end_base = 0x0;

using tess_begin_t = void(__fastcall*)(__int64 _this, char one, int four, char zero, __int64 alsoZero);
using tess_vertex_t = void(__fastcall*)(__int64 _this, float v1, float v2, float v3);
using tess_end_t = void(__fastcall*)(__int64, __int64 tesselator, __int64*);

tess_begin_t tess_begin = reinterpret_cast<tess_begin_t>(Utils::FindSignature("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC ?? 80 B9 ?? ?? ?? ?? 00 41 0F"));
tess_vertex_t tess_vertex = reinterpret_cast<tess_vertex_t>(Utils::FindSignature("4C 8B DC 55 53 49 8D 6B ?? 48 81 EC ?? ?? ?? ?? 41"));
tess_end_t tess_end = reinterpret_cast<tess_end_t>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 49 8B D8 4C 8B E2 4C 8B F9 80"));

void DrawUtils::setCtx(C_MinecraftUIRenderContext * ctx, C_GuiData* gui)
{
	guiData = gui;
	renderCtx = ctx;
	a2 = reinterpret_cast<__int64*>(renderCtx)[2];
	tesselator = *reinterpret_cast<__int64*>(a2 + 0x78);
	colorHolder = *reinterpret_cast<float**>(a2 + 0x30);

	if (tess_end_base == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("FF 50 08 4C 8D 05") + 3;
		int offset = *reinterpret_cast<int*>(sigOffset + 3);
		tess_end_base = reinterpret_cast<__int64*>(sigOffset + offset + 7);
	}
}

void DrawUtils::setColor(float r, float g, float b, float a)
{
	colorHolder[0] = r;
	colorHolder[1] = g;
	colorHolder[2] = b;
	colorHolder[3] = a;
	*reinterpret_cast<uint8_t*>(colorHolder + 4) = 1;
}

void DrawUtils::flush()
{
	renderCtx->flushText(0);
}

void DrawUtils::drawLine(vec2_t start, vec2_t end, float lineWidth)
{
	float modX = 0 - (start.y - end.y);
	float modY = start.x - end.x;

	float len = sqrtf(modX * modX + modY * modY);

	modX /= len;
	modY /= len;
	modX *= lineWidth;
	modY *= lineWidth;

	tess_begin(tesselator, 3, 0, 1, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, start.x - modX, start.y - modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_vertex(tesselator, start.x + modX, start.y + modY, 0);
	tess_vertex(tesselator, end.x + modX, end.y + modY, 0);
	tess_vertex(tesselator, end.x - modX, end.y - modY, 0);

	tess_end(a2, tesselator, tess_end_base);

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
}
