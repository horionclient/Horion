#include "InventoryMove.h"



InventoryMove::InventoryMove() : IModule(0x0, MOVEMENT)
{
}


InventoryMove::~InventoryMove()
{
}

const char* InventoryMove::getModuleName()
{
	return ("InventoryMove");
}

void InventoryMove::onTick(C_GameMode* gm)
{
}