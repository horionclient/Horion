#pragma once
#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"

class CrystalAura : public IModule {
private:
	int delay = 0;
	int eRange = 3;
	int range = 10;
	bool autoplace = false;
	bool pEnhanced = true;
	bool dEnhanced = true;

public:
	CrystalAura();
	~CrystalAura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onEnable() override;
	virtual void onDisable() override;

	int cRange = 10;
	int pRange = 5;

	void CPlace(C_GameMode* gm, vec3_t* pos);
	void CDestroy(C_Entity* ent);
};