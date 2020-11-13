#include "VanillaPlus.h"
#include "../../../Utils/Utils.h"
#include <Windows.h>

#include "../../../Utils/Logger.h"

unsigned char flyingItemsPrevCodeBuf[26];
unsigned char* flyingItemsFuncPtr = 0;

bool lastFastFlyingItems = false;

VanillaPlus::VanillaPlus() : IModule(0, Category::MISC, "Tweaks the minecraft experience") {
	registerBoolSetting("Fast Item Lerp", &fastFlyingItems, true);
}

const char* VanillaPlus::getModuleName() {
	return "Vanilla+";
}

void VanillaPlus::onEnable() {
	// flying items
	{
		if (flyingItemsFuncPtr == nullptr)
			flyingItemsFuncPtr = reinterpret_cast<unsigned char*>(9 + FindSignature("48 3B D8 0F 84 F8 01 00 00"));
		memcpy(flyingItemsPrevCodeBuf, flyingItemsFuncPtr, 26);

		DWORD oldProtect = 0;
		if (!VirtualProtect(flyingItemsFuncPtr, 26, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
#endif
		} else {
			auto code = std::initializer_list<unsigned char>({0xB9, 0x00, 0x00, 0x00, 0x00, 0x66, 0x44, 0x0F, 0x6E, 0xC9, 0xB9, 0x00, 0x00, 0x00, 0x00, 0x66, 0x44, 0x0F, 0x6E, 0xD9, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90});
			std::copy(code.begin(), code.end(), flyingItemsFuncPtr);
			*reinterpret_cast<float*>(&flyingItemsFuncPtr[1]) = 0.15f;
			*reinterpret_cast<float*>(&flyingItemsFuncPtr[11]) = 1 / 0.15f;
			VirtualProtect(flyingItemsFuncPtr, 26, oldProtect, &oldProtect);
		}
	}
}


void VanillaPlus::onTick(C_GameMode*) {
	if (flyingItemsFuncPtr && lastFastFlyingItems != fastFlyingItems) {
		DWORD oldProtect = 0;
		if (!VirtualProtect(flyingItemsFuncPtr, 26, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
#endif
		} else {
			float val = fastFlyingItems ? 0.025f : 0.15f;
			*reinterpret_cast<float*>(&flyingItemsFuncPtr[1]) = val;
			*reinterpret_cast<float*>(&flyingItemsFuncPtr[11]) = 1 / val;
			VirtualProtect(flyingItemsFuncPtr, 26, oldProtect, &oldProtect);
		}


		lastFastFlyingItems = fastFlyingItems;
	}
}


void VanillaPlus::onDisable() {
	// flying items
	if (flyingItemsFuncPtr) {
		DWORD oldProtect = 0;
		if (!VirtualProtect(flyingItemsFuncPtr, 26, PAGE_EXECUTE_READWRITE, &oldProtect)) {
#ifdef _DEBUG
			logF("couldnt unprotect memory send help");
			__debugbreak();
#endif
		} else {
			memcpy(flyingItemsFuncPtr, flyingItemsPrevCodeBuf, 26);
			VirtualProtect(flyingItemsFuncPtr, 26, oldProtect, &oldProtect);
		}
	}
}
