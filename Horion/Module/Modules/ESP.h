#pragma once

#include "Module.h"
#include "../../DrawUtils.h"

class ESP : public IModule
{
public:
	ESP();
	~ESP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

