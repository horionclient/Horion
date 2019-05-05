#pragma once

#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Utils/HMath.h"

class DrawUtils {
public:
	static void setCtx(C_MinecraftUIRenderContext* ctx);
	static void render();

	static void drawLine(vec2_t start, vec2_t end, float lineWidth, float* color); // rgba
};