#include "TriggerBot.h"



TriggerBot::TriggerBot() : IModule('K', COMBAT) // <-- keybind
{
}


TriggerBot::~TriggerBot()
{
}

const char* TriggerBot::getModuleName()
{
	return ("TriggerBot");
}

void TriggerBot::onTick(C_GameMode* gm)
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	C_Entity* target = g_Data.getClientInstance()->getPointerStruct()->entityPtr;
	if (target != 0)
	{
		if (localPlayer->getEntityTypeId() == target->getEntityTypeId())
		{
			localPlayer->swingArm();
			gm->attack(target);
		}
			
	}
}


