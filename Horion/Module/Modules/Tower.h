#pragma once
#include "Module.h"

class Tower : public IModule {
private:
	float motion = 0.5f;
	bool tryTower(vec3_t blockBelow);

public:
	Tower();
	~Tower();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
