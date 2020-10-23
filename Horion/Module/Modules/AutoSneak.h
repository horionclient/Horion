#pragma once
#include "Module.h"
class AutoSneak : public IModule {
private:
	

public:
	bool doSilent = false;
	AutoSneak();
	~AutoSneak();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
	virtual void onSendPacket(C_Packet* packet) override;
	virtual void onEnable() override;
};
