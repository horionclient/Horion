#pragma once

#include "ScriptManager.h"
#include "Vector3Functions.h"


class LocalPlayerFunctions {
public:
	static JsValueRef prototype;

	DECL_FUN(setPosition);
	DECL_FUN(setVelocity);
	DECL_FUN(toString);
	DECL_FUN(setViewAngles);
	DECL_FUN(setIsOnGround);
};
