#pragma once
#include "Module.h"
class HitboxSize : public IModule {
private:
	float playersize = 1;
	bool active = false;
	float ogsize = 0;

public:
	HitboxSize();
	~HitboxSize();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};