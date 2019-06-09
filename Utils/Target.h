#pragma once

#include "../SDK/CEntity.h"

class Target {

public:
	static void init(C_LocalPlayer** lc);
	static bool isValidTarget(C_Entity* ent);
};