#include "GuiUtils.h"


void GuiUtils::drawCrossLine(vec4_t pos, MC_Color col,float lineWidth,bool secondCross) {
	float Midx = (pos.z + pos.x) / 2;
	float Midy = (pos.y + pos.w) / 2;
	DrawUtils::setColor(col.r,col.g,col.b,col.a);
	DrawUtils::drawLine(vec2_t(pos.x, Midy), vec2_t(pos.z, Midy), lineWidth);
	if(secondCross)
		DrawUtils::drawLine(vec2_t(Midx, pos.y), vec2_t(Midx, pos.w), lineWidth);
}
