#include "ChestStealer.h"



ChestStealer::ChestStealer() : IModule(0x0, PLAYER)
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
	if (g_Data.getLocalPlayer()->isInventoryClosed() == 0 && chestScreenController != nullptr)
	{ 
		for (int i = 0; i < 54; i++)
		{
			chestScreenController->handleAutoPlace(0x7FFFFFFF,"container_items", i);
		}
		delay++;
	}
	if (g_Data.getLocalPlayer()->isInventoryClosed() == 0 && chestScreenController != nullptr && delay > 7)
	{
		chestScreenController->leaveScreen();
		delay = 0;
	}
		
	chestScreenController = nullptr;
}


