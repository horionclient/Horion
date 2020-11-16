#pragma once

#include "../../../Utils/HMath.h"
#include "../ScriptManager.h"
#include <optional>

class Vector3Functions {
public:
	static std::optional<vec3_t> getVec3FromValue(JsValueRef);
	static std::optional<vec3_t> getVec3FromArguments(JsValueRef*, int argCount, int* nextArg = nullptr);

	DECL_FUN(isValid);
	DECL_FUN(getX);
	DECL_FUN(getY);
	DECL_FUN(getZ);
	DECL_FUN(toString);
	DECL_FUN(constructor);

	DECL_FUN(add);
	DECL_FUN(sub);
	DECL_FUN(div);
	DECL_FUN(mul);
};
