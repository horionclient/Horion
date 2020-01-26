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
	bool isOtherCheckEnabled() {
		return otherCheck && this->isEnabled();
	}
	bool isExtraCheckEnabled() {
		return this->extraCheck;
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
