#pragma once

#include "../ScriptManager.h"

class DrawFunctions {
public:
	DECL_FUN(drawLine3d);
	DECL_FUN(setColor);
	DECL_FUN(drawLinestrip3d);
	DECL_FUN(drawLine2d);
	DECL_FUN(drawRectangle2d);
	DECL_FUN(fillRectangle2d);

	DECL_FUN(drawText2d);
	DECL_FUN(getTextWidth);
	DECL_FUN(getTextLineHeight);

	DECL_FUN(getWindowSize);
	DECL_FUN(getOrigin);
};
