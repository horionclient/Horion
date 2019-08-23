#include "TriggerBot.h"
#include "../../../Utils/Target.h"



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
		if (target == localPlayer) // Skip Local player
			return;

		if (target == 0)
			return;

		if (target->timeSinceDeath > 0 || target->damageTime >= 7)
			return;

		if (FriendList::findPlayer(target->getNameTag()->getText())) // Skip Friend
			return;

		if (!Target::isValidTarget(target))
			return;

		if (localPlayer->getEntityTypeId() == target->getEntityTypeId())
		{
			localPlayer->swingArm();
			gm->attack(target);
		}
		Odelay = 0;
		}
	}


