#pragma once
#include "Module.h"
class AntiBot :
	public IModule
{
private:
	bool hitboxCheck = true;
	bool nameCheck = true;
	bool invisibleCheck = true;
	bool entityIdCheck = true;
	bool immobileCheck = true;
public:
	AntiBot();
	~AntiBot();

	bool isHitboxCheckEnabled() {
		return hitboxCheck && this->isEnabled();
	}
	bool isNameCheckEnabled() {
		return nameCheck && this->isEnabled();
	}
	bool isInvisibleCheckEnabled() {
		return invisibleCheck && this->isEnabled();
	}
	bool isEntityIdCheckEnabled() {
		return entityIdCheck && this->isEnabled();
	}
	bool isImmobileCheckEnabled() {
		return immobileCheck && this->isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};

