#pragma once
#include "Module.h"
class HitboxSize : public IModule {
private:
	float width = 0.5;
	float height = 0.5;
	bool active = false;
	float ogwidth;
	float ogheight;

public:
	HitboxSize();
	~HitboxSize();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};