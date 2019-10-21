#include "FastLadder.h"



FastLadder::FastLadder() : IModule(0x0, Category::MOVEMENT, "Climb up ladders faster")
{
}


FastLadder::~FastLadder()
{
}

const char* FastLadder::getModuleName()
{
	return ("FastLadder");
}

void FastLadder::onEnable()
{
	if (opcode == 0) {
		static uintptr_t sigOffset = Utils::FindSignature("C7 87 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 07 48 8B CF FF 90 ?? ?? ?? ?? 84 C0 74 10");
		opcode = reinterpret_cast<float*>(sigOffset + 6);
	}
		

	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 4, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
	else {
		*opcode = -0.4f;
		VirtualProtect(opcode, 4, oldProtect, &oldProtect);
	}

}

void FastLadder::onDisable()
{
	if (opcode == 0) {
		static uintptr_t sigOffset = Utils::FindSignature("C7 87 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 07 48 8B CF FF 90 ?? ?? ?? ?? 84 C0 74 10");
		opcode = reinterpret_cast<float*>(sigOffset + 6);
	}
	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 4, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
	else {
		*opcode = -0.2f;
		VirtualProtect(opcode, 4, oldProtect, &oldProtect);
	};
}