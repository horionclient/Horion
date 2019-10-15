#include "Target.h"
#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity * ent)
{
	if (ent == NULL)
		return false;

	if (ent == g_Data.getLocalPlayer())
		return false;

	if (ent->getEntityTypeId() > 60 && ent->getEntityTypeId() < 104 && ent->getEntityTypeId() != 63) // check for non mob entity ids
		return false;

	static Hitbox* hitboxMod = moduleMgr->getModule<Hitbox>();
	if (hitboxMod == NULL)
		hitboxMod = moduleMgr->getModule<Hitbox>();

	if (FriendList::findPlayer(ent->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled())
		return false;

	if (ent->getNameTag()->getTextLength() <= 1)
		return false;

	if(!hitboxMod->isEnabled())
		if (ent->height < 1.5f || ent->width < 0.5f || ent->height > 2.1f || ent->width > 0.9f)
			return false;

	if (ent->isInvisible())
		return false;

	if (!(*localPlayer)->canAttack(ent, false))
		return false;

	return true;
}
