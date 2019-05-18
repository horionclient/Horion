#pragma once
#include "Module.h"
#include "../../DrawUtils.h"
class Scaffold :
	public IModule
{
private:
	bool tryScaffold(vec3_t blockBelow);
public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual std::string getModuleName() override;
	virtual void onPostRender() override;
};

