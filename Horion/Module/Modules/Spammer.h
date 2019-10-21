#pragma once
#include "Module.h"

class Spammer : public IModule
{
private:
	int Odelay = 0;
public:
	Spammer();
	~Spammer();

	std::string random();
	bool bypass = true;
	int delay = 2;
	std::string message = "Horion - the best minecraft bedrock utility mod - horionclient.eu";

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};