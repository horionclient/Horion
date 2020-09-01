#pragma once
#include "Horion/Module/Modules/Module.h"

class AirStuck : public IModule {
public:
	AirStuck();
	~AirStuck();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
