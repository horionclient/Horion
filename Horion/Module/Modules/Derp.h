#pragma once
#include "Module.h"
class Derp : public IModule {
private:
	int counter = 0;
	bool epicStroke = false;
	bool packetMode = false;

public:
	Derp();
	~Derp();

	// Inherited via IModule
	virtual void onTick(C_Player* player) override;
	virtual const char* getModuleName() override;
};
