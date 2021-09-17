#pragma once
#include "Module.h"
class Scaffold : public IModule {
private:
	bool spoof = false;
	bool staircaseMode = false;
	bool tryScaffold(vec3_t blockBelow);
	bool findBlock();
	bool AutoSelect = false;
	bool invalidSelect = false;

public:
	Scaffold();
	~Scaffold();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onLevelRender() override;
	virtual void onDisable() override;
};
