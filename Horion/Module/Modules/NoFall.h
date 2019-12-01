#pragma once

#include "Module.h"
#include "../../DrawUtils.h"


class NoFall : public IModule
{
public:
	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};

