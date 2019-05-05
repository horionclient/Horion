#include "DrawUtils.h"

C_MinecraftUIRenderContext* renderCtx;

void DrawUtils::setCtx(C_MinecraftUIRenderContext * ctx)
{
	renderCtx = ctx;
}

void DrawUtils::render()
{
	renderCtx->flushText(0);
}

void DrawUtils::drawLine(vec2_t start, vec2_t end, float lineWidth, float * color)
{
}
