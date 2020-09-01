#pragma once

#include "Horion/DrawUtils.h"
#include "Horion/Module/Modules/Module.h"

class NoFall : public IModule {
public:
	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};
