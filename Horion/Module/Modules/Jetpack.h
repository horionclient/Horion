#pragma once
#include "Module.h"

class Jetpack : public IModule {
private:
	int delay = 0;
	bool isBypass = false;
	float speedMod = 1;

public:
	C_MovePlayerPacket* jetpack = nullptr;
	Jetpack();
	~Jetpack();

	virtual void onTick(C_GameMode* gm) override;
	virtual bool isFlashMode() override;

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
