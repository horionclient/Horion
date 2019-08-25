#include "AutoClicker.h"


AutoClicker::AutoClicker() : IModule(0x0, COMBAT) // <-- keybind
{
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerBoolSetting("only swords/axes", &this->sword, this->sword);
	//this->registerBoolSetting("debug", &this->debug, this->debug);
}


AutoClicker::~AutoClicker()
{
}

const char* AutoClicker::getModuleName()
{
	return ("AutoClicker");
}

void AutoClicker::onTick(C_GameMode* gm)
{
	if (!g_Data.isLeftClickDown() && Odelay != 0) Odelay = 0;
	if (g_Data.isLeftClickDown() && GameData::canUseMoveKeys()) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		C_Entity* target = g_Data.getClientInstance()->getPointerStruct()->entityPtr;
		Odelay++;

		if (Odelay >= delay)
		{
			if (sword && !(localPlayer->itemId == 268 || localPlayer->itemId == 267 || localPlayer->itemId == 272 || localPlayer->itemId == 276 || localPlayer->itemId == 283 /*swords*/ || localPlayer->itemId == 271 || localPlayer->itemId == 275 || localPlayer->itemId == 279 || localPlayer->itemId == 286 || localPlayer->itemId == 258 /*axes*/)) {
				return;
			}

			localPlayer->swingArm();

			if (target != 0)
			{
				//if (debug)
					//g_Data.getGuiData()->displayClientMessageF("%s %f %f", target->getNameTag()->getText(), target->width, target->height);
				gm->attack(target);
			}
			Odelay = 0;
		}
	}
}
