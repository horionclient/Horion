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

	inline bool isHitboxCheckEnabled() {
		return hitboxCheck && this->isEnabled();
	}
	inline bool isNameCheckEnabled() {
		return false && this->isEnabled();
	}
	inline bool isInvisibleCheckEnabled() {
		return invisibleCheck && this->isEnabled();
	}
	inline bool isEntityIdCheckEnabled() {
		return entityIdCheck && this->isEnabled();
	}
	inline bool isOtherCheckEnabled() {
		return otherCheck && this->isEnabled();
	}
	inline bool isExtraCheckEnabled() {
		return this->extraCheck && this->isEnabled();
	}

	// Inherited via IModule
	virtual const char* getModuleName() override;
};
