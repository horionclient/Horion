#include "TriggerBot.h"



TriggerBot::TriggerBot() : IModule('K', COMBAT) // <-- keybind
{
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
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
	Odelay++;
	if (target != 0 && Odelay >= delay)
	{
		if (localPlayer->getEntityTypeId() == target->getEntityTypeId())
		{
			localPlayer->swingArm();
			gm->attack(target);
		}
		Odelay = 0;	
	}
}


