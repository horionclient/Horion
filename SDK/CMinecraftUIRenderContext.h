#pragma once

#include "TextHolder.h"

class C_MinecraftUIRenderContext {
public:
	float idk[140];

	virtual ~C_MinecraftUIRenderContext();
	virtual float getLineLength(uintptr_t font, TextHolder* str, float textSize, bool idk);
	virtual float getTextAlpha();
	virtual void setTextAlpha(float alpha);
	virtual __int64 drawDebugText(const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const void* caretMeasureData);
	virtual __int64 drawText(uintptr_t font, const float* pos, TextHolder* text, float* color, float alpha, unsigned int textAlignment, const float* textMeasureData, const uintptr_t* caretMeasureData);
	virtual void flushText(float timeSinceLastFlush); // time used for ticking the obfuscated text
	virtual __int64 drawImageNOTIMPLEMENTED(); // didnt bother putting in the parameters
	virtual __int64 drawNinesliceNOTIMPLEMENTED();
	virtual __int64 flushImagesNOTIMPLEMENTED();
	virtual __int64 beginSharedMeshBatchNOTIMPLEMENTED();
	virtual __int64 endSharedMeshBatchNOTIMPLEMENTED();
	virtual void drawRectangle(const float* pos, const float* color, float alpha, int lineWidth); // line width is guessed
	virtual void fillRectangle(const float* pos, const float* color, float alpha);
	// There are a few more functions but didnt bother
};