#pragma once
#include "Module.h"
class BowAimbot : public IModule {
private:
	bool silent = true;
	vec2_t angle;

public:
	BowAimbot();
	~BowAimbot();

	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
	virtual void onSendPacket(C_Packet* packet) override;
};
