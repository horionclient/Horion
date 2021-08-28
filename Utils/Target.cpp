#include "Target.h"

#include <regex>

#include "../Horion/Module/ModuleManager.h"

C_LocalPlayer** localPlayer;

void Target::init(C_LocalPlayer** cl) {
	localPlayer = cl;
}

bool Target::isValidTarget(C_Entity* ent) {
	if (ent == nullptr)
		return false;

	auto localPlayer = g_Data.getLocalPlayer();

	if (ent == localPlayer)
		return false;

	static auto antibot = moduleMgr->getModule<AntiBot>();
	static auto hitboxMod = moduleMgr->getModule<Hitbox>();
	static auto teams = moduleMgr->getModule<Teams>();
	static auto noFriends = moduleMgr->getModule<NoFriends>();

	if (!ent->isAlive())
		return false;

	auto entityTypeId = ent->getEntityTypeId();

	if (antibot->isEntityIdCheckEnabled() && entityTypeId <= 130 && entityTypeId != 63)
		return false;

	if (entityTypeId == 63) {
		if (teams->isColorCheckEnabled()) {
			auto targetName = ent->getNameTag();
			auto localName = localPlayer->getNameTag();
			if (targetName->getTextLength() > 2 && localName->getTextLength() > 2) {
				auto colorTargetName = std::regex_replace(targetName->getText(), std::regex("\\§r"), "");
				auto colorLocalName = std::regex_replace(localName->getText(), std::regex("\\§r"), "");
				if (colorTargetName.at(0) == colorLocalName.at(0)) 
					return false;
			}
		}
		if (teams->isAlliedCheckEnabled()) {
			if (localPlayer->isAlliedTo(ent)) return false;
		}
	}

	// Temporarily removed from gui, tons of false negatives
	if (antibot->isNameCheckEnabled() && !Target::containsOnlyASCII(ent->getNameTag()->getText()))
		return false;

	if (!noFriends->isEnabled() && FriendList::findPlayer(ent->getNameTag()->getText()))
		return false;

	if (antibot->isInvisibleCheckEnabled() && ent->isInvisible() )
		return false;

	if (antibot->isOtherCheckEnabled() && (ent->isSilent() || ent->isImmobile() || ent->getNameTag()->getTextLength() < 1 || std::string(ent->getNameTag()->getText()).find(std::string("\n")) != std::string::npos))
		return false;

	if (!hitboxMod->isEnabled() && antibot->isHitboxCheckEnabled())
		if ((ent->height < 1.5f || ent->width < 0.49f || ent->height > 2.1f || ent->width > 0.9f))
			return false;

	if (!localPlayer->canAttack(ent, false))
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
