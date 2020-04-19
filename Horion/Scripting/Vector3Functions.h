#pragma once

#include "../../Utils/HMath.h"
#include "ScriptManager.h"
#include <optional>

class Vector3Functions {
private:
	static std::optional<vec3_t> getVecFromValue(JsValueRef);

public:
	static std::optional<vec3_t> getVecFromArguments(JsValueRef*, int argCount);

	static JsValueRef prototype;

	DECL_FUN(isValid);
	DECL_FUN(getX);
	DECL_FUN(getY);
	DECL_FUN(getZ);
	DECL_FUN(toString);
	DECL_FUN(constructor);
};
