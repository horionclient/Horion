#pragma once
#include"DrawUtils.h"

// Place buttons in here idk

class GuiUtils
{
public:
	static void drawCrossLine(vec2_t pos, MC_Color col, float lineWidth, float crossSize, bool secondCross);
	static void checkBox();
	static void drawWindow(const char* windowName,vec2_t pos);
};

