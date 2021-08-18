#pragma once
#include "Module.h"
class ViewModel : public IModule {
private:
	int delay = 0;

public:
	bool Reset = false;
	bool doTranslate = true;
	bool doScale = true;

	float float1 = 0;
	float xMod = 1.f;
	float yMod = 1.f;
	float zMod = 1.f;

	float xTrans = 0.f;
	float yTrans = 0.f;
	float zTrans = 0.f;

	ViewModel();
	~ViewModel();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
