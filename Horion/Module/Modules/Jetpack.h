#pragma once
#include "Module.h"

class Jetpack : public IModule {
private:
	bool selfhurt = false;
	int delay = 0;
	bool isBypass = false;
	float speedMod = 1;

public:
	C_MovePlayerPacket* jetpack = nullptr;
	Jetpack();
	~Jetpack();

	virtual bool isFlashMode() override;

	// Inherited via IModule
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual const char* getModuleName() override;
};
