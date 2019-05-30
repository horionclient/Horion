#include "TriggerBot.h"



TriggerBot::TriggerBot() : IModule('K', COMBAT) // <-- keybind
{
}


TriggerBot::~TriggerBot()
{
}

std::string TriggerBot::getModuleName()
{
	return std::string("TriggerBot");
}

void TriggerBot::onTick(C_GameMode* gm)
{
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	uintptr_t target1 = g_Data.getClientInstance()->getPointerStruct()->entityPtr;
	if (target1 != 0)
	{
		C_Entity* target = reinterpret_cast<C_Entity*>(target1);
		if (localPlayer->getEntityTypeId() == target->getEntityTypeId())
		{
			localPlayer->swingArm();
			gm->attack(target);
		}
			
	}
}


