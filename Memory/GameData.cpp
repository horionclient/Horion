#include "GameData.h"

GameData g_Data;

void GameData::retrieveClientInstance()
{
	static uintptr_t clientInstanceOffset = 0x0;
	if (clientInstanceOffset == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("4C 8B F8 48 8B 0D ?? ?? ?? ?? 48 8B 11");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 6)); // Get Offset from code
			clientInstanceOffset = sigOffset - g_Data.gameModule->ptrBase + offset + /*length of instruction*/ 7 + 3; // Offset is relative
			logF("clinet: %llX", clientInstanceOffset);
		} 
	}
	g_Data.clientInstance = reinterpret_cast<C_ClientInstance*>(g_Data.slimMem->ReadPtr<uintptr_t*>(g_Data.gameModule->ptrBase + clientInstanceOffset, { 0x0, 0x298, 0x8 }));
	// 4C 8B F8 48 8B 0D ?? ?? ?? ?? 48 8B 11
	// 1.11.1 : 0x0250A2D0
}

bool GameData::isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (keyMapOffset == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 89 3C 81 E9");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3)); // Get Offset from code
			keyMapOffset = sigOffset - g_Data.gameModule->ptrBase + offset + /*length of instruction*/ 7; // Offset is relative
		}
	}
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(g_Data.gameModule->ptrBase + keyMapOffset + (key * 0x4));
}
bool GameData::isKeyPressed(int key) {
	if (isKeyDown(key)) {
		while (isKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}

void GameData::updateGameData(C_GameMode * gameMode)
{
	retrieveClientInstance();
	g_Data.localPlayer = g_Data.getLocalPlayer();
	if (gameMode->player == g_Data.localPlayer) { // GameMode::tick might also be run on the local server
		g_Data.gameMode = gameMode;
		if (g_Data.localPlayer != 0x0) {
			auto *vecLock = Logger::GetTextToPrintSection();
			
			if (vecLock == nullptr || TryEnterCriticalSection(vecLock)) {
				auto* stringPrintVector = Logger::GetTextToPrint();
				
				for (std::vector<TextForPrint>::iterator it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
					C_GuiData* guiData = g_Data.clientInstance->getGuiData();
					
					guiData->displayClientMessageF("%s%s%s%s", GOLD, it->time, RESET, it->text);
				}
				stringPrintVector->clear();
				if(vecLock != nullptr)
					LeaveCriticalSection(vecLock);
			}
		}
	}
}
void GameData::Chest_tick(C_ChestBlockActor * chest)
{
	std::set<std::shared_ptr<AABB>>::iterator it;

	for (it = g_Data.chestList.begin(); it != g_Data.chestList.end(); ++it) 
		if ((**it) == chest->aabb) 
			return;

	std::shared_ptr<AABB> toAdd = std::make_shared<AABB>(chest->aabb);
	g_Data.chestList.insert(toAdd);
}

void GameData::initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem)
{
	g_Data.gameModule = gameModule;
	g_Data.slimMem = slimMem;
	retrieveClientInstance();
#ifdef _DEBUG
	logF("clientInstance %llX", g_Data.clientInstance);
#endif
}
