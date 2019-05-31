#pragma once

#include "CEntity.h"
#include "CLoopbackPacketSender.h"
#include "TextHolder.h"

class Minecraft;
class LevelRenderer {
private:
	char pad_0x0000[0x7D0]; //0x0000
public:
	vec3_t origin; //0x07D0 
};
class HitDetectSystem;

class MinecraftGame {
private:
	char filler[0xA0];
	uintptr_t goodFont;
	char pad_0x00A8[0x80]; //0x00A8
	bool canUseKeys;
public:
	
	uintptr_t getTheGoodFontThankYou() {
		return goodFont;
	};

	const bool canUseKeybinds() {
		return canUseKeys;
	};
};

class C_GuiData {
private:
	char pad_0x0000[0x18]; //0x0000
public:
	float widthReal; //0x0018 
	float heightReal; //0x001C 
	float widthReal2; //0x0020 
	float heightReal2; //0x0024 
	float widthGame; //0x0028 
	float heightGame; //0x002C 

	void displayClientMessageVA(const char * fmt, va_list lis) {
		char bigboi[300];
		int numCharacters = vsprintf_s(bigboi, 300, fmt, lis);
		displayClientMessage(&std::string(bigboi));
	}

	void displayClientMessageF(const char * fmt, ...) {
		va_list arg;
		va_start(arg, fmt);
		displayClientMessageVA(fmt, arg);
		va_end(arg);
	}
	void displayClientMessage(std::string* a2) {
		using displayClientMessage = void(__thiscall*)(void*, TextHolder); // This signature actually exists 2 times but we got luck that our function is the first in memory
		static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 4C 8B FA 4C 8B E9 48 89 4C 24 ?? 48 C7 45 ?? 0F 00 00 00 33 C0 48 89 45 ?? 88 45"));
		
		TextHolder text = TextHolder(*a2);
		
		displayMessageFunc(this, text);
	};
};

class C_ClientInstance {
private:
	char firstPad[0x48]; //0x0000
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

	PointingStruct* getPointerStruct() {
		return Utils::CallVFunc<140, PointingStruct*>(this);
	}

	glmatrixf* getRefDef() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);

		return reinterpret_cast<glmatrixf*>(_this + 0x258);
	};

	vec2_t* getMousePos(){
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		vec2_t* MousePos;
		MousePos = reinterpret_cast<vec2_t*>(_this + 0x3C0);
		return MousePos;
	}

	vec2_t getFov() {
		uintptr_t _this = reinterpret_cast<uintptr_t>(this);
		vec2_t fov;
		fov.x = *reinterpret_cast<float*>(_this + 0x5B8);
		fov.y = *reinterpret_cast<float*>(_this + 0x5CC);
		return fov;
	}

	C_GuiData* getGuiData() {
		return Utils::CallVFunc<177, C_GuiData*>(this);
	};

	uintptr_t getFont() {
		
		return Utils::CallVFunc<73, uintptr_t>(this);
	}

	uintptr_t getRuneFont() {

		return Utils::CallVFunc<74, uintptr_t>(this);
	}

	uintptr_t getUnicodeFont() {

		return Utils::CallVFunc<75, uintptr_t>(this);
	}

	void grabMouse() {
		Utils::CallVFunc<239, void>(this);
	}

	void releaseMouse() {
		Utils::CallVFunc<240, void>(this);
	}
};