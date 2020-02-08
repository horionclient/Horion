#pragma once

#include "Module.h"

class NameTags : public IModule {
public:
	bool displayArmor = true;
	NameTags();
	~NameTags();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender(C_MinecraftUIRenderContext* renderCtx) override;
};
