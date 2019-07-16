#pragma once
#include "Module.h"
#include "../../DrawUtils.h"

class InfiniteReach : public IModule
{
public:
	int delay = 0;
	float range = 15;
	bool isMulti = true;
	C_MovePlayerPacket* pack = nullptr;
	InfiniteReach();
	~InfiniteReach();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};

