#pragma once
#include "Module.h"

class AutoTotem : public IModule {
private:
	int delay = 0;

public:
	AutoTotem();
	~AutoTotem();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
};
