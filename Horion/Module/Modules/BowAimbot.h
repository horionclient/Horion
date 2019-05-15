#pragma once
#include "Module.h"
class BowAimbot :
	public IModule
{
public:
	BowAimbot();
	~BowAimbot();

	virtual std::string getModuleName() override;
	virtual void onPostRender() override;
};

