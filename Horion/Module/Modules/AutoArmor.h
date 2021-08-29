#pragma once
#include "Module.h"
class AutoArmor : public IModule {
public:
	C_CraftingScreenController* inventoryScreen = nullptr;
	AutoArmor();
	~AutoArmor();

	virtual void onTick(C_Player* plr) override;
	virtual const char* getModuleName() override;
};
