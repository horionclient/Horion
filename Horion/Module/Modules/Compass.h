#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
class Compass : public IModule {
private:
	float opacity = 0.5;
	int range = 90;
	void drawCenteredText(vec2_t pos, std::string text, float size, float textOpacity = 1);

public:
	Compass();
	~Compass();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
