#pragma once
#include "Module.h"
class Blink :
	public IModule
{
public:
	std::vector<C_MovePlayerPacket*> PacketBlink;
	Blink();
	~Blink();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
};
