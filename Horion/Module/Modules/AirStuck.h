#pragma once
#include "Module.h"

class AirStuck : public IModule {
private:
	bool selfhurt = false;
	
public:
	AirStuck();
	~AirStuck();

	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
