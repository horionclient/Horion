#pragma once
#include "Horion/Module/Modules/Module.h"
class AutoSneak : public IModule {
public:
	AutoSneak();
	~AutoSneak();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
