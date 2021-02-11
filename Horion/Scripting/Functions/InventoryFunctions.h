#pragma once

#include "../ScriptManager.h"

class InventoryFunctions {
public:
	DECL_FUN(getItems);
	DECL_FUN(getArmor);
	DECL_FUN(getHeld);
	DECL_FUN(getSlot);
	DECL_FUN(moveItem);
	DECL_FUN(setSelected);

	DECL_FUN(isFull);
};
