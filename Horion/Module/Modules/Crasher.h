#pragma once
#pragma once
#include "Module.h"
class Crasher : public IModule {
public:
	Crasher();
	~Crasher();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};
