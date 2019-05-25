#pragma once

#include "Module.h"
#include "../../DrawUtils.h"


class NoFall : public IModule
{
public:
	NoFall();
	~NoFall();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

