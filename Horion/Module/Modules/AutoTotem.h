#pragma once
#include "Module.h"

class AutoTotem : public IModule {
public:
	AutoTotem();
	~AutoTotem();

	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
};
