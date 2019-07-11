#include "ChestStealer.h"



ChestStealer::ChestStealer() : IModule(0x0, EXPLOITS)
{
}


ChestStealer::~ChestStealer()
{

}

const char* ChestStealer::getModuleName()
{
	return ("ChestStealer");
}

void ChestStealer::onTick(C_GameMode* gm)
{
	C_ChestScreenController** chestScreenController = g_Data.getChestScreenController();
	if (*chestScreenController != NULL)
	{ 
		for (int i = 0; i < 54; i++)
		{
			(*chestScreenController)->handleAutoPlace(0x7FFFFFFF,"container_items", i);
		}
	}
	*chestScreenController = NULL;
}


