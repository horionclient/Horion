#pragma once
#include "Module.h"
class Compass : public IModule {
private:
	float opacity = 0.5;
	void drawCenteredText(vec2_t pos, std::string text, float size);

public:
	Compass();
	~Compass();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};