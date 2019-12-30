#pragma once
#include "Module.h"

class Nuker : public IModule {
private:
	int nukerRadius = 4;
	bool veinMiner = false;

public:
	Nuker();
	~Nuker();

	inline int getNukerRadius() { return nukerRadius; };

	inline bool isVeinMiner() { return veinMiner; };
	// Inherited via IModule
	virtual const char* getModuleName() override;
};
