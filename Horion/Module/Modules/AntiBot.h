#pragma once
#include "Module.h"
class AntiBot : public IModule {
private:
	bool hitboxCheck = true;
	bool nameCheck = false;
	bool invisibleCheck = true;
	bool entityIdCheck = true;
	bool otherCheck = true;
	bool extraCheck = false;
	bool teams = false;

public:
	AntiBot();
	~AntiBot();

	bool isHitboxCheckEnabled() {
		return hitboxCheck && this->isEnabled();
	}
	bool isNameCheckEnabled() {
		return /*nameCheck*/false && this->isEnabled();
	}
	bool isInvisibleCheckEnabled() {
		return invisibleCheck && this->isEnabled();
	}
	bool isEntityIdCheckEnabled() {
		return entityIdCheck && this->isEnabled();
	}
	bool isOtherCheckEnabled() {
		return otherCheck && this->isEnabled();
	}
	bool isExtraCheckEnabled() {
		return this->extraCheck && this->isEnabled();
	}
	bool isTeamsEnabled() {
		return this->teams && this->isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
