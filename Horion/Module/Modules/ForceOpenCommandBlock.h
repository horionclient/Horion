#pragma once
#include "Module.h"

class ForceOpenCommandBlock : public IModule {
private:
	bool clicked = false;

public:
	ForceOpenCommandBlock();
	~ForceOpenCommandBlock();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
