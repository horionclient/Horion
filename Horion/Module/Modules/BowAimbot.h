#pragma once
#include "Module.h"
class BowAimbot :
	public IModule
{
public:
	bool requireClick = false;
	BowAimbot();
	~BowAimbot();

	virtual const char* getModuleName() override;
	virtual void onPostRender() override;
};

