#pragma once

#include <vector>

#include "../SDK/CEntity.h"

class Target {
private:
	static bool containsOnlyASCII(const std::string& string);

public:
	static void init(C_LocalPlayer** lc);
	static bool isValidTarget(C_Entity* ent);
};
