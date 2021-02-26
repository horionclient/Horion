#pragma once
#include "Module.h"
class Fly : public IModule {
private:
	float speed = 1.5f;
	int gameTick = 0;

public:
	Fly();
	~Fly();

	SettingEnum mode;

	// Inherited via IModule
	virtual bool isFlashMode() override;
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
