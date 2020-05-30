#include "GuiUtils.h"

void GuiUtils::drawCrossLine(vec2_t pos, MC_Color col, float lineWidth, float crossSize, bool secondCross) {
	crossSize /= 2;
	DrawUtils::setColor(col.r, col.g, col.b, col.a);
	//float MidX = (pos.z + pos.x) / 2;
	//float MidY = (pos.y + pos.w) / 2;
	DrawUtils::drawLine(vec2_t(pos.x - crossSize, pos.y), vec2_t(pos.x + crossSize, pos.y), lineWidth);
	if (secondCross)
		DrawUtils::drawLine(vec2_t(pos.x, pos.y - crossSize), vec2_t(pos.x, pos.y + crossSize), lineWidth);
}
