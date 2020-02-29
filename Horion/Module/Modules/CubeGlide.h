#pragma once
#include "Module.h"
class CubeGlide : public IModule {
private:
	float speed = 1.5f;
	int gameTick = 0;

public:
	CubeGlide();
	~CubeGlide();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual bool isFlashMode() override;
	virtual void onEnable() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onDisable() override;
};
