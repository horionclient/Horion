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

bool GameData::canUseMoveKeys()
{
	MinecraftGame* mc = g_Data.clientInstance->minecraftGame;
	if(mc == nullptr)
		return false;
	return mc->canUseKeybinds();
}

bool GameData::isKeyDown(int key) {
	static uintptr_t keyMapOffset = 0x0;
	if (keyMapOffset == 0x0) {
		uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 89 3C 81 E9");
		if (sigOffset != 0x0) {
			int offset = *reinterpret_cast<int*>((sigOffset + 3)); // Get Offset from code
			keyMapOffset = sigOffset - g_Data.gameModule->ptrBase + offset + /*length of instruction*/ 7; // Offset is relative
#ifdef _DEBUG
			logF("KeyMap: %llX", keyMapOffset + g_Data.gameModule->ptrBase);
#endif
		}
	}
	// All keys are mapped as bools, though aligned as ints (4 byte)
	// key0 00 00 00 key1 00 00 00 key2 00 00 00 ...
	return *reinterpret_cast<bool*>(g_Data.gameModule->ptrBase + keyMapOffset + ((uintptr_t) key * 0x4));
}
bool GameData::isKeyPressed(int key) {
	if (isKeyDown(key)) {
		while (isKeyDown(key))
			Sleep(1);
		return true;
	}
	return false;
}

bool GameData::isLeftClickDown() {
	if (g_Data.hidController == 0)
		return false;
	return g_Data.hidController->leftClickDown;
}

bool GameData::shouldTerminate()
{
	return g_Data.shouldTerminateB;
}

void GameData::terminate()
{
	g_Data.shouldTerminateB = true;
}

void GameData::updateGameData(C_GameMode * gameMode)
{
	retrieveClientInstance();
	g_Data.localPlayer = g_Data.getLocalPlayer();

	if (gameMode->player == g_Data.localPlayer) { // GameMode::tick might also be run on the local server
		g_Data.gameMode = gameMode;
		
		if (g_Data.localPlayer != nullptr ) {
			C_GuiData* guiData = g_Data.clientInstance->getGuiData();
			auto *vecLock = Logger::GetTextToPrintSection();
			
			if (guiData != nullptr && (vecLock == nullptr || TryEnterCriticalSection(vecLock))) {
				auto* stringPrintVector = Logger::GetTextToPrint();
				
				for (std::vector<TextForPrint>::iterator it = stringPrintVector->begin(); it != stringPrintVector->end(); ++it) {
					guiData->displayClientMessageF("%s%s%s%s", GOLD, it->time, RESET, it->text);
				}
				stringPrintVector->clear();
				if(vecLock != nullptr)
					LeaveCriticalSection(vecLock);
			}
		}
	}
}

void GameData::EntityList_tick(C_EntityList * list)
{
	g_Data.entityList = list;
}

void GameData::setHIDController(C_HIDController* Hid)
{
	g_Data.hidController = Hid;
}

void GameData::forEachEntity(void(*callback)(C_Entity *))
{
	C_LocalPlayer* localPlayer = getLocalPlayer();

	// New EntityList
	/*{
		// MultiplayerLevel::directTickEntities
		// 48 89 5C 24 08 48 89 74  24 18 57 48 83 EC 20 48 8B 7A 20 48 8B F2 48 8B  BF F8 01 00 00 48 8B 1F
		__int64 region = reinterpret_cast<__int64>(g_Data.getLocalPlayer()->region);
		__int64* entityIdMap = *(__int64 **)(*(__int64 *)(region + 0x20) + 0x1F8i64);
		for (__int64 *i = (__int64 *)*entityIdMap; i != entityIdMap; i = (__int64 *)*i)
		{
			__int64 actor = i[3];
			if (actor && !*(char *)(actor + 0xD09) && !*(char *)(actor + 0xD0A)) {
				C_Entity* ent = reinterpret_cast<C_Entity*>(actor);
				callback(ent);
			}
		}
	}*/


	// Regular EntityList
	{
		C_EntityList* entList = g_Data.getEntityList();
		size_t listSize = entList->getListSize();
		if (listSize < 1000 && listSize > 1) {
			for (size_t i = 0; i < listSize; i++) {
				C_Entity* current = entList->get(i);
				callback(current);
			}
		}
	}
}

void GameData::addChestToList(C_ChestBlockActor * chest)
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
	logF("localPlayer %llX", g_Data.getLocalPlayer());
#endif
}
