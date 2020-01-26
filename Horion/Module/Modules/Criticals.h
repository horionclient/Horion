#pragma once
#include "Module.h"
class Criticals : public IModule {
public:
	Criticals();
	~Criticals();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};
