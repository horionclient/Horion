#pragma once

#include "TextHolder.h"

class C_Font {
private:
	virtual void destructorFont();
	virtual void unk1();
	virtual void unk2();
	virtual void unk3();
	virtual void unk4();
	virtual void unk5();

public:
	virtual float getLineLength(TextHolder* str, float textSize, bool unknown);
	virtual float getLineHeight();
};

class C_MinecraftUIRenderContext {
private:
	float _[140];

public:
	virtual ~C_MinecraftUIRenderContext();
	virtual float getLineLength(C_Font* font, TextHolder* str, float textSize, bool unknown);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual __int64 drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(C_Font* font, const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush);  // time used for ticking the obfuscated text
	virtual __int64 drawImageNOTIMPLEMENTED();         // didnt bother putting in the parameters
	virtual __int64 drawNinesliceNOTIMPLEMENTED();
	virtual __int64 flushImagesNOTIMPLEMENTED();
	virtual __int64 beginSharedMeshBatchNOTIMPLEMENTED();
	virtual __int64 endSharedMeshBatchNOTIMPLEMENTED();
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth);  // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	// There are a few more functions but didnt bother
};
