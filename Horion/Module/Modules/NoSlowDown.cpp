#include "NoSlowDown.h"



NoSlowDown::NoSlowDown() : IModule(0x0, Category::MOVEMENT, "Don't get slowed down when blocking or eating")
{
}


NoSlowDown::~NoSlowDown()
{
}

const char* NoSlowDown::getModuleName()
{
	return ("NoSlowDown");
}

void NoSlowDown::onEnable()
{
	if (opcode == 0)
		opcode = reinterpret_cast<uint8_t*>(Utils::FindSignature("2F 48 85 DB ?? 2A F3") - 1);

	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
	else {
		*opcode = 0xEB;
		VirtualProtect(opcode, 1, oldProtect, &oldProtect);
	}
	
}

void NoSlowDown::onDisable()
{
	if (opcode == 0)
		opcode = reinterpret_cast<uint8_t*>(Utils::FindSignature("2F 48 85 DB ?? 2A F3") - 1);

	DWORD oldProtect = 0;
	if (!VirtualProtect(opcode, 1, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
		logF("couldnt unprotect memory send help");
		__debugbreak();
#endif
	}
	else {
		*opcode = 0x75;
		VirtualProtect(opcode, 1, oldProtect, &oldProtect);
	};
}