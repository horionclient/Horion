#pragma once

#include "../../Utils/HMath.h"
#include "ScriptManager.h"
#include <optional>

class Vector3Functions {
private:
	static std::optional<vec3_t> getVecFromValue(JsValueRef);

public:
	DECL_FUN(isValid);
};
