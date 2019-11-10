#include "Reach.h"


Reach::Reach() : IModule(0x0, Category::COMBAT, "Increases your reach")
{
	this->registerFloatSetting("Reach Value", &this->reachValue, this->reachValue,3.f,7.f);
}


Reach::~Reach()
{
}

const char* Reach::getModuleName()
{
	return ("Reach");
}

void Reach::onTick(C_GameMode* gm)
{
	if (reachPtr != 0) {
		*reachPtr = reachValue;
	}
}

void Reach::onEnable()
{
	static uintptr_t sigOffset = 0x0;
	if (sigOffset == 0x0) {
		sigOffset = Utils::FindSignature("EB 18 F3 0F 10 05 ?? ?? ?? ?? 44 0F");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 6)); // Get Offset from code
			reachPtr = reinterpret_cast<float*>(sigOffset + offset + 10);
		}
	}
	if (!VirtualProtect(reachPtr, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
	
}

void Reach::onDisable()
{
	if (reachPtr != 0)
		VirtualProtect(reachPtr, sizeof(float), oldProtect, &oldProtect);
}