#include "Reach.h"


Reach::Reach() : IModule(0x0, COMBAT) // <-- keybind
{
	this->registerFloatSetting("Reach Value", &this->reachValue, this->reachValue,3,7);
}


Reach::~Reach()
{
}

const char* Reach::getModuleName()
{
	return ("Reach");
}

void  Reach::onTick(C_GameMode* gm)
{
	static uintptr_t sigOffset = 0x0;
	if (sigOffset == 0x0) {
		sigOffset = Utils::FindSignature("EB 18 F3 0F 10 05 ?? ?? ?? ?? 44 0F 2F D0");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 6)); // Get Offset from code
			reachPtr = reinterpret_cast<float*>(sigOffset  + offset + 10); 
			logF("reachPtr: %llX", reachPtr);
		}
	}
}


