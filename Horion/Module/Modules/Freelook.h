#pragma once
#include "Module.h"

class Freelook : public IModule {
private:
	vec2_t initialViewAngles = {};
public:
	int resetViewTick = -1;
	vec2_t lastCameraAngle = {0, 0};
	bool redirectMouse = false;
	bool isThirdPerson = false;
	bool cameraFacesFront = false;

	Freelook();
	~Freelook();

	virtual const char* getModuleName() override;
	bool isFlashMode() override {
		return true;
	}
	void onTick(C_GameMode* mode) override;
	void onEnable() override;
	void onDisable() override;
	void onPostRender(C_MinecraftUIRenderContext* ctx) override;
	bool callWhenDisabled() override {
		return true;
	}
};
