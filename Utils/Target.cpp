#include "Target.h"
#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;
std::vector<std::string> validEntities;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity * ent)
{
	if (ent == 0)
		return false;

	static Hitbox* hitboxMod = moduleMgr->getModule<Hitbox>();
	if (hitboxMod == 0)
		hitboxMod = moduleMgr->getModule<Hitbox>();

	const bool isPlayer = ent->getEntityTypeId() == 63;

	if (isPlayer)
		return false;

	if (ent->getNameTag()->getTextLength() <= 1)
		return false;

	if(!hitboxMod->isEnabled())
		if (ent->height < 1.5f || ent->width < 0.5f || ent->height > 2.1f || ent->width > 0.9f)
			return false;

	if (ent->isInvisible() && ent->getEntityTypeId() != 33) // Exception for kitmap.sylphhcf.net they use a creeper as hitbox
		return false;

	if (!(*localPlayer)->canAttack(ent, false))
		return false;

	if (validEntities.size() > 2)
	{
		for (auto it : validEntities)
		{
			if (it.find(ent->uuid.getText()) != std::string::npos)
				return true;
		}
	}

	if (*ent->getPos() == *ent->getPosOld()/* && reinterpret_cast<C_Player*>(ent)->getSupplies()->inventory == NULL*/)
		return false;
	
	validEntities.push_back(ent->uuid.getText());

	if (validEntities.size() >= 100)
		validEntities.clear();

	return true;
}
