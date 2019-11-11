#pragma once
#include "Module.h"
class PacketLogger :
	public IModule
{
public:
	PacketLogger();
	~PacketLogger();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

