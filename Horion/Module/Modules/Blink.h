#pragma once
#include "Module.h"
class Blink :
	public IModule
{
private:
	std::vector<C_MovePlayerPacket> PacketMeme;
public:
	
	Blink();
	~Blink();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	void AddPackets(C_MovePlayerPacket* a);
	void sendPackets();
};

