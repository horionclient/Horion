#pragma once
#include "Horion/Module/Modules/Module.h"
class NoPacket : public IModule {
public:
	NoPacket();
	~NoPacket();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	bool allowAutoStart() override;
	void onEnable() override;
};
