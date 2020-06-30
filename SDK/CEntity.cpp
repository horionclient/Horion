#include "CEntity.h"
#include "../Utils/Utils.h"
#include "../Memory/GameData.h"
C_InventoryTransactionManager *C_Entity::getTransactionManager() {
	static unsigned int offset = 0;
	if (offset == 0) {
		// EnchantCommand::execute
		//offset = *reinterpret_cast<int *>(FindSignature("48 8D 8B ?? ?? ?? ?? E8 ?? ?? ?? ?? 90 48 8D 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 8D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B ?? ?? ?? 8B") + 3);
		offset = *reinterpret_cast<int *>(FindSignature("49 8D 8E ? ? ? ? E8 ? ? ? ? 90 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ?") + 3);
	}
	return reinterpret_cast<C_InventoryTransactionManager *>(reinterpret_cast<__int64>(this) + offset);
}
C_PlayerInventoryProxy *C_Player::getSupplies() {
	static unsigned int offset = 0;
	if (offset == 0) {
		//offset = *reinterpret_cast<int *>(FindSignature("4C 8B 82 ?? ?? ?? ?? 48 8B B2") + 3);  // GameMode::startDestroyBlock -> GameMode::_canDestroy -> getSupplies
		if (g_Data.getVersion() == GAMEVERSION::g_1_16_0)
			offset = *reinterpret_cast<int *>(FindSignature("48 8B 53 ? 48 8B F0 4C 8B 8A ? ? ? ? 41 80 B9 ? ? ? ? ? 75 18 49 8B 89 ? ? ? ? 41 8B 51 ? 4C 8B 01 41 FF 50") + 10);  // GameMode::startDestroyBlock -> GameMode::_canDestroy -> getSupplies
		else
			offset = *reinterpret_cast<int *>(FindSignature("48 8B 51 ? 4C 8B 82 ? ? ? ? 48 8B B2 ? ? ? ? 41 80 B8 ? ? ? ? ? ") + 7);  // GameMode::startDestroyBlock -> GameMode::_canDestroy -> getSupplies
	}
	return *reinterpret_cast<C_PlayerInventoryProxy **>(reinterpret_cast<__int64>(this) + offset);
}
void C_LocalPlayer::unlockAchievments() {  // MinecraftEventing::fireEventAwardAchievement
	using fireEventAward = void(__fastcall *)(void *, int);
	static fireEventAward fireEventAwardFunc = reinterpret_cast<fireEventAward>(FindSignature("48 85 C9 0F 84 ?? ?? ?? ?? 55 56 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 8B"));
	for (int i = 0; i < 100; i++)
		fireEventAwardFunc(this, i);
}
void C_LocalPlayer::applyTurnDelta(vec2_t *viewAngleDelta) {
	using applyTurnDelta = void(__thiscall *)(void *, vec2_t *);
	static applyTurnDelta TurnDelta = reinterpret_cast<applyTurnDelta>(FindSignature("48 89 5C 24 ?? 57 48 83 EC ?? 48 8B D9 0F 29 74 24 ?? 48 8B 89 ?? ?? ?? ?? 48 8B ?? 0F 29 7C 24 ?? 44 0F"));
	TurnDelta(this, viewAngleDelta);
}
void C_LocalPlayer::setGameModeType(int gma) {
	// Player::setPlayerGameType
	//using setGameMode = void(__thiscall *)(void *, int);
	//static setGameMode Game_Mode = reinterpret_cast<setGameMode>(FindSignature("48 8B C4 56 57 41 56 48 81 EC ? ? ? ? 48 C7 44 24 ? ? ? ? ? 48 89 58 ? 48 89 68 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 8B EA 48 8B D9 44 8B B1 ? ? ? ? 83 FA ? 75 2D"));
	this->setC_PlayerGameType(gma);
	//Game_Mode(this, gma);
}
