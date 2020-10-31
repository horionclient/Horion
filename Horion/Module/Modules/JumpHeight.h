#pragma once
#include "Module.h"
class JumpHeight : public IModule {
public:
	float jumpPower = 0.42f; //normal jump height (1.25 blocks)
	JumpHeight();
	~JumpHeight();

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
