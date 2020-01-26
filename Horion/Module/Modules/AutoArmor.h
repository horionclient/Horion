#pragma once
#include "Module.h"
class AutoArmor : public IModule {
public:
	C_CraftingScreenController* inventoryScreen = nullptr;
	AutoArmor();
	~AutoArmor();

	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
