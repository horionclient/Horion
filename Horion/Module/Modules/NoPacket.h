#pragma once
#include "Module.h"
class NoPacket :
	public IModule
{
public:
	NoPacket();
	~NoPacket();

	// Inherited via IModule
	virtual std::string getModuleName() override;
};

