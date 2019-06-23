#include "GuiUtils.h"


void GuiUtils::drawCrossLine(vec4_t pos, MC_Color col, float lineWidth, bool secondCross) {
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	float MidX = (pos.z + pos.x) / 2;
	float MidY = (pos.y + pos.w) / 2;
	DrawUtils::drawLine(vec2_t(pos.x, MidY), vec2_t(pos.z, MidY), lineWidth);
	if(secondCross)
		DrawUtils::drawLine(vec2_t(MidX, pos.y), vec2_t(MidX, pos.w), lineWidth);
}
