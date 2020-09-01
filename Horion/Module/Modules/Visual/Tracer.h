#pragma once
#include "Horion/Module/Modules/Module.h"
class Tracer : public IModule {
public:
	Tracer();
	~Tracer();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	void onLevelRender() override;
};
