#pragma once
#include "Module.h"

class HudModule : public IModule {
public:
	HudModule();
	~HudModule();

	bool tabgui = true;
	bool arraylist = true;
	bool clickToggle = true;
	bool watermark = true;
	bool coordinates = false;
	bool keybinds = true;
	bool displayArmor = true;
	bool keystrokes = true;
	bool fps = true;
	bool cps = true;
	bool alwaysShow = false;

	float scale = 1.f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
