#pragma once

#include "CEntity.h"

class MinecraftGame;
class Minecraft;
class LevelRenderer;
class LoopbackPacketSender;
class HitDetectSystem;


class C_ClientInstance {
private:
	char pad_0x0000[0x40]; //0x0000
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
	LoopbackPacketSender* loopbackPacketSender; //0x0080 
private:
	char pad_0x0088[0x28]; //0x0088
public:
	HitDetectSystem* hitDetectSystem; //0x00B0 
private:
	char pad_0x00B8[0x28]; //0x00B8
public:
	C_LocalPlayer* localPlayer; //0x00E0 

public:
	C_LocalPlayer* getLocalPlayer() {
		return localPlayer;
	}
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