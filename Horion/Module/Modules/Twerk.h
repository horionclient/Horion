#pragma once
#include "Module.h"
class Twerk : public IModule {
public:
	Twerk();
	~Twerk();

private:
	bool isSneaking = false;
	int Odelay = 0;
	float delay = 1;
	inline float& getDelay() { return delay; };
	virtual const char* getModuleName() override;
	void onTick(C_GameMode* gm);
};
