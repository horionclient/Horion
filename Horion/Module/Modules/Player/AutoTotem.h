#pragma once
#include "Horion/Module/Modules/Module.h"

class AutoTotem : public IModule {
public:
	AutoTotem();
	~AutoTotem();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
