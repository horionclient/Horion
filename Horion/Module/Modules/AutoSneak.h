#pragma once
#include "Module.h"
class AutoSneak : public IModule {
public:
	AutoSneak();
	~AutoSneak();

	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
	virtual const char* getRawModuleName() override;
};
