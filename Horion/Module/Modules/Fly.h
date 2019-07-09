#pragma once
#include "Module.h"
class Fly :
	public IModule
{
private:
	int oldGameMode;
public:
	Fly();
	~Fly();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};

