#pragma once

#include "CEntity.h"
#include "CLoopbackPacketSender.h"
#include "TextHolder.h"

class MinecraftGame;
class Minecraft;
class LevelRenderer;
class HitDetectSystem;

class C_GuiData {
public:
	__int64 displayClientMessage(char* a2) {
		using displayClientMessage = __int64(__thiscall*)(void*, char*); // This signature actually exists 2 times but we got luck that our function is the first in memory
		static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(Utils::FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 ?? 4C 8B FA 4C 8B E9 48 89 4C 24 ?? 48 C7 45 ?? 0F 00 00 00 33 C0 48 89 45 ?? 88 45"));
		__debugbreak();
		return displayMessageFunc(this, a2);
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
};

/*
class unusued
{
public:
	C_ClientInstance* clientInstance; //0x0000 
private:
	char pad_0x0008[0x10]; //0x0008
public:
	void* minecraftClientScriptEngine; //0x0018 

}; //Size=0x0020*/