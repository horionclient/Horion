#pragma once
#include "Module.h"
class Tracer : public IModule {
public:
	Tracer();
	~Tracer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};
