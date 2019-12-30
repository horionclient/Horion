#pragma once

#include "../../DrawUtils.h"
#include "Module.h"

class ChestESP : public IModule {
private:
	int tickTimeout = 0;

public:
	ChestESP();
	~ChestESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
	virtual void onTick(C_GameMode* gm) override;
};
