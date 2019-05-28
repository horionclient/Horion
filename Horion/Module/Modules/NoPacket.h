#pragma once
#include "Module.h"
class NoPacket :
	public IModule
{
public:
	std::vector<C_MovePlayerPacket*> PacketMeme;
	NoPacket();
	~NoPacket();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

