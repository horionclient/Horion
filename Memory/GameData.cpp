#include "GameData.h"

GameData g_Data;

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
	g_Data.clientInstance = reinterpret_cast<C_ClientInstance*>(g_Data.slimMem->ReadPtr<uintptr_t*>(g_Data.gameModule->ptrBase + 0x0250A2D0, { 0x0, 0x298, 0x8 }));
	//logF("clientInstance = %llX", g_Data.clientInstance);
	g_Data.localPlayer = g_Data.getLocalPlayer();
	if (gameMode->player == g_Data.localPlayer) { // GameMode::tick might also be run on the local server
		g_Data.gameMode = gameMode;
		if (g_Data.localPlayer != 0x0) {
			auto *vecLock = Logger::GetTextToPrintSection();
			
			if (TryEnterCriticalSection(vecLock)) {
				
				auto* stringPrintVector = Logger::GetTextToPrint();
				
				for (std::vector<TextForPrint>::iterator it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
					C_GuiData* guiData = g_Data.clientInstance->getGuiData();
				
					guiData->displayClientMessageF("%s%s%s%s", GOLD, it->time, RESET, it->text);
					//g_Data.localPlayer->displayClientMessageStr(std::string(yeetus)); //std::string(it->text, it->length)
				}
				
				stringPrintVector->clear();
				LeaveCriticalSection(vecLock);
			}
				
			
		}
		
	}
}

void GameData::initGameData(const SlimUtils::SlimModule* gameModule, SlimUtils::SlimMem* slimMem)
{
	g_Data.gameModule = gameModule;
	g_Data.slimMem = slimMem;
	g_Data.clientInstance = g_Data.slimMem->ReadPtr<C_ClientInstance*>(g_Data.gameModule->ptrBase + 0x0250A2D0, { 0x0, 0x298, 0x8 });
}
