#pragma once
#include "Horion/Module/Modules/Module.h"

class NoHurtcam : public IModule{
public:
	NoHurtcam();
	~NoHurtcam();
	const char* getModuleName() override;
};
