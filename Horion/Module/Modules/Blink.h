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
	virtual std::string getModuleName() override;
};

