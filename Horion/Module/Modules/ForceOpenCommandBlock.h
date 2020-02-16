#pragma once
#include "Module.h"

class ForceOpenCommandBlock : public IModule {
private:
	bool clicked = false;

public:
	ForceOpenCommandBlock();
	~ForceOpenCommandBlock();

	bool isInCommandBlock = false;
	float distance = 7;

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
