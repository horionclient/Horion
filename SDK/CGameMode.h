#pragma once

#include <stdint.h>

#include "../Memory/MinHook.h"
#include "../Utils/HMath.h"
#include "CClientInstance.h"
#include "CEntity.h"

class C_GameMode {
private:
	virtual __int64 destructorGameMode();
	// Duplicate destructor
public:
	virtual __int64 startDestroyBlock(vec3_ti const &, unsigned char, bool &);
	virtual __int64 destroyBlock(vec3_ti *, unsigned char);
	virtual __int64 continueDestroyBlock(vec3_ti const &, unsigned char, bool &);
	virtual __int64 stopDestroyBlock(vec3_ti const &);
	virtual __int64 startBuildBlock(vec3_ti const &, unsigned char);
	virtual __int64 buildBlock(vec3_ti *, unsigned char);
	virtual __int64 continueBuildBlock(vec3_ti const &, unsigned char);
	virtual __int64 stopBuildBlock(void);
	virtual __int64 tick(void);

private:
	virtual __int64 getPickRange(__int64 const &, bool);
	virtual __int64 useItem(__int64 &);
	virtual __int64 useItemOn(__int64 &, vec3_ti const &, unsigned char, vec3_t const &, __int64 const *);
	virtual __int64 interact(C_Entity &, vec3_t const &);

public:
	virtual __int64 attack(C_Entity *);

private:
	virtual __int64 releaseUsingItem(void);

public:
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);

private:
	virtual __int64 registerUpsellScreenCallback(__int64);

public:
	C_Entity *player;
};
