#pragma once

#include "Module.h"

class ChestESP : public IModule {
private:
	int tickTimeout = 0;
	std::vector<std::shared_ptr<AABB>> bufferedChestList;
	std::mutex listLock;

public:
	ChestESP();
	~ChestESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onTick(C_GameMode* gm) override;
};
