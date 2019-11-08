#pragma once
#include "Module.h"
class Blink :
	public IModule
{
public:
	std::vector<C_MovePlayerPacket*> PacketMeme;
	Blink();
	~Blink();

	// Inherited via IModule
	virtual void onTick(C_GameMode* gm);
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
};

