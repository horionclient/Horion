#include "Target.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity * ent)
{
	if (ent == 0)
		return false;

	const bool isPlayer = ent->getEntityTypeId() == 63;

	if (ent->getNameTag()->getTextLength() <= 1 && isPlayer)
		return false;
	if (ent->height < 1.5f || ent->width < 0.5f || ent->height > 2.1f || ent->width > 0.9f)
		return false;
	if (ent->isInvisible() && ent->getEntityTypeId() != 33) // Exception for kitmap.sylphhcf.net they use a creeper as hitbox
		return false;
	if (!(*localPlayer)->canAttack(ent, false))
		return false;
	
	return true;
}
