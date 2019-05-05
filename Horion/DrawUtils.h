#pragma once

#include "../SDK/CMinecraftUIRenderContext.h"
#include "../SDK/CClientInstance.h"
#include "../Utils/HMath.h"
#include "../Utils/Utils.h"
#include "../Utils/Logger.h"

class DrawUtils {
public:
	static void setCtx(C_MinecraftUIRenderContext* ctx, C_GuiData* guiData);
	static void flush();
	static void setColor(float r, float g, float b, float a); // rgba, values from 0 to 1

	static void drawLine(vec2_t start, vec2_t end, float lineWidth); // rgba
	static void Begin(char one, int four, char zero, __int64 alsoZero);
	static void End();

	static void wirebox(AABB aabb);
};