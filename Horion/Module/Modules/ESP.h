#pragma once

#include "Module.h"
#include "../../DrawUtils.h"

class ESP : public IModule
{
public:
	ESP();
	~ESP();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onPostRender() override;
};

