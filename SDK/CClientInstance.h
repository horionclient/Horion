#pragma once

#include "CEntity.h"
#include "CLoopbackPacketSender.h"
#include "TextHolder.h"

class Minecraft;
class LevelRenderer;
class HitDetectSystem;

class MinecraftGame {
private:
	char filler[0xA0];
	uintptr_t goodFont;
public:
	
	uintptr_t getTheGoodFontThankYou() {
		return goodFont;
	}
};

class C_GuiData {
public:
	void displayClientMessageF(const char * fmt, ...) {
		char bigboi[300];
		va_list arg;
		va_start(arg, fmt);
		int numCharacters = vsprintf_s(bigboi, 300, fmt, arg);
		va_end(arg);
		displayClientMessage(&std::string(bigboi));
	}
	void displayClientMessage(std::string* a2) {
		using displayClientMessage = void(__thiscall*)(void*, TextHolder); // This signature actually exists 2 times but we got luck that our function is the first in memory
		static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 4C 8B FA 4C 8B E9 48 89 4C 24 ?? 48 C7 45 ?? 0F 00 00 00 33 C0 48 89 45 ?? 88 45"));
		
		TextHolder* text = new TextHolder(*a2);
		
		displayMessageFunc(this, *text);
	};
};

class C_ClientInstance {
private:
	char pad_0x0000[0x48]; //0x0000
public:
	MinecraftGame* minecraftGame; //0x0040 
private:
	MinecraftGame* N0000080C; //0x0048 
	MinecraftGame* N0000080D; //0x0050 
	MinecraftGame* N0000080E; //0x0058 
public:
	Minecraft* minecraft; //0x0060 
private:
	char pad_0x0068[0x8]; //0x0068
public:
	LevelRenderer* levelRenderer; //0x0070 
private:
	char pad_0x0078[0x8]; //0x0078
public:
	C_LoopbackPacketSender* loopbackPacketSender; //0x0080 
private:
	char pad_0x0088[0x28]; //0x0088
public:
	HitDetectSystem* hitDetectSystem; //0x00B0 
private:
	char pad_0x00B8[0x28]; //0x00B8
public:
	C_LocalPlayer* localPlayer; //0x00E0 

	C_LocalPlayer* getLocalPlayer() {
		return localPlayer;
	};

	C_GuiData* getGuiData() {
		return Utils::CallVFunc<177, C_GuiData*>(this);
	};

	void* getFont() {
		
		return Utils::CallVFunc<73, void*>(this);
	}

	void* getRuneFont() {

		return Utils::CallVFunc<74, void*>(this);
	}

	void* getUnicodeFont() {

		return Utils::CallVFunc<75, void*>(this);
	}

	void grabMouse() {
		Utils::CallVFunc<239, void>(this);
	}

	void releaseMouse() {
		Utils::CallVFunc<240, void>(this);
	}
};