#pragma once
#include "Module.h"

class StackableItem : public IModule {
public:
	int maxSize = 64;
	StackableItem();
	~StackableItem();

	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
