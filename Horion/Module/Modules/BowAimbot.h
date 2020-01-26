#pragma once
#include "Module.h"
class BowAimbot : public IModule {
public:
	BowAimbot();
	~BowAimbot();

	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};
