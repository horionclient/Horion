#pragma once

#include "../ScriptManager.h"

class InventoryFunctions {
public:
	DECL_FUN(getItems);
	DECL_FUN(getArmor);
	DECL_FUN(getHeld);

	DECL_FUN(isFull);
};
