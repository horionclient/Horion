#include "Target.h"
#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity* ent) {
	if (ent == NULL)
		return false;

	if (ent == g_Data.getLocalPlayer())
		return false;

	static AntiBot* antibot = moduleMgr->getModule<AntiBot>();
	if (antibot == NULL) antibot = moduleMgr->getModule<AntiBot>();
	static Hitbox* hitboxMod = moduleMgr->getModule<Hitbox>();
	if (hitboxMod == NULL) hitboxMod = moduleMgr->getModule<Hitbox>();

	if (!ent->isAlive())
		return false;

	if (ent->getEntityTypeId() <= 122 && ent->getEntityTypeId() != 63 && antibot->isEntityIdCheckEnabled())
		return false;

	if (!Target::containsOnlyASCII(ent->getNameTag()->getText()) && antibot->isNameCheckEnabled())
		return false;

	if (FriendList::findPlayer(ent->getNameTag()->getText()) && !moduleMgr->getModule<NoFriends>()->isEnabled())
		return false;

	if (ent->isInvisible() && antibot->isInvisibleCheckEnabled())
		return false;

	if ((ent->isSilent() || ent->isImmobile() /*ent->getNameTag()->getTextLength() < 1*/ || std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos) && antibot->isOtherCheckEnabled())
		return false;

	if (!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled())
		if ((ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
			return false;

	if (!(*localPlayer)->canAttack(ent, false))
		return false;

	if (antibot->isExtraCheckEnabled() && !ent->canShowNameTag())
		return false;

	return true;
}

bool Target::containsOnlyASCII(const std::string& string) {
	for (auto c : string) {
		int n = static_cast<unsigned char>(c);
		if (n > 127 && n != -89) {
			return false;
		}
	}
	return true;
}