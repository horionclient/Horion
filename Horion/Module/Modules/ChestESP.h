#pragma once

#include "Module.h"
#include "../../DrawUtils.h"

class ChestESP : public IModule
{
public:
	ChestESP();
	~ChestESP();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onPostRender() override;
};

