#pragma once

#include "Module.h"
#include "../../DrawUtils.h"
#include "../../../Utils/Target.h"

class MobESP : public IModule
{
public:
	MobESP();
	~MobESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

