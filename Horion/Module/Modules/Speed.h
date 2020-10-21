#pragma once
#include "Module.h"

class Speed : public IModule {
private:
	float origSpeed = 0;
	float speed = 1.6f;
	float stage = 0;
	float boost = 4.1f;
	float timerspeed = 20.0f;
	bool motion = false;
	bool custom = true;
	bool timer = false;

public:
	Speed();
	~Speed();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
