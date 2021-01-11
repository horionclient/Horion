#include "Loader.h"

SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif

DWORD WINAPI keyThread(LPVOID lpParam) {
	logF("Key thread started");

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if (keyMap == nullptr)
		throw std::exception("Keymap not allocated");

	auto clickMap = reinterpret_cast<uintptr_t>(malloc(5));
	if (clickMap == 0)
		throw std::exception("Clickmap not allocated");

	bool* keyMapAddr = nullptr;
	uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 89 1C B9");
	if (sigOffset != 0x0) {
		int offset = *reinterpret_cast<int*>((sigOffset + 3));                                   // Get Offset from code
		keyMapAddr = reinterpret_cast<bool*>(sigOffset + offset + /*length of instruction*/ 7);  // Offset is relative
	} else {
		logF("!!!KeyMap not located!!!");
		throw std::exception("Keymap not located");
	}

	C_HIDController** hidController = g_Data.getHIDController();

	while (isRunning) {
		if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::shouldTerminate()) {  // Press L to uninject
			isRunning = false;
			break;
		}

		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i);
			bool newKeyPressed = (*newKey) && GameData::canUseMoveKeys();  // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int)i, newKeyPressed);
			}
			if (*newKey != *oldKey) {  // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
				ClickGui::onKeyUpdate((int)i, *newKey);
			}
		}

		if (*hidController != nullptr) {
			for (uintptr_t key = 0; key < 5; key++) {
				bool newKey = (*hidController)->clickMap[key];
				bool* oldKey = reinterpret_cast<bool*>(clickMap + key);
				if (newKey != *oldKey) {
					ClickGui::onMouseClickUpdate((int)key, newKey);
					HImGui.onMouseClickUpdate((int)key, newKey);
					if (newKey) {
						if ((int)key == 0)
							g_Data.leftclickCount++;
						else if ((int)key == 1)
							g_Data.rightclickCount++;
					}
				}
			}

			memcpy(reinterpret_cast<void*>(clickMap), &(*hidController)->leftClickDown, 5);
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);

		Sleep(2);
	}
	logF("Stopping Threads...");
	GameData::terminate();
	Sleep(200);  // Give the threads a bit of time to exit

	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);  // Uninject
}

DWORD WINAPI injectorConnectionThread(LPVOID lpParam) {
	logF("Injector Connection Thread started");

	struct MemoryBoi {
		short protocolVersion;
		bool isPresent;
		bool isUnread;
		int clientVersion;
		DATAPACKET_CMD cmd;
		int params[5];
		int dataSize;
		unsigned char data[3000];
		unsigned char zeroByte = 0;
	};

	unsigned char magicValues[16] = {0x00, 0x4F, 0x52, 0x00, 0x49, 0x4F, 0x4E, 0x23, 0x9C, 0x47, 0xFB, 0xFF, 0x7D, 0x9C, 0x42, 0x57};
	char* magicArray = new char[sizeof(magicValues) + sizeof(uintptr_t) * 2];
	memcpy(magicArray, magicValues, sizeof(magicValues));

	logF("Magic array at %llX", magicArray);

	auto** horionToInjectorPtr = reinterpret_cast<MemoryBoi**>(magicArray + sizeof(magicValues));
	auto** injectorToHorionPtr = reinterpret_cast<MemoryBoi**>(magicArray + sizeof(magicValues) + sizeof(uintptr_t));

	*horionToInjectorPtr = new MemoryBoi();
	MemoryBoi* horionToInjector = *horionToInjectorPtr;
	horionToInjector->isPresent = true;
	horionToInjector->protocolVersion = 1;

	*injectorToHorionPtr = new MemoryBoi();
	MemoryBoi* injectorToHorion = *injectorToHorionPtr;

	magicArray[0] = 0x48;  //Only find this allocated one, not the one in the thread stack

	logF("horionToInjectorPtr at %llX", horionToInjector);
	logF("injectorToHorionPtr at %llX", injectorToHorion);

	LARGE_INTEGER frequency, timeSinceLastMessage, timeSinceLastPing;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&timeSinceLastMessage);
	QueryPerformanceCounter(&timeSinceLastPing);

	bool loggedIn = false;

	while (isRunning) {
		Sleep(2);
		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);
		bool isConnected = horionToInjector->isPresent && injectorToHorion->isPresent && horionToInjector->protocolVersion >= injectorToHorion->protocolVersion;

		if (isConnected && !injectorToHorion->isUnread) {
			__int64 elapsed = endTime.QuadPart - timeSinceLastMessage.QuadPart;
			float realElapsed = (float)elapsed / frequency.QuadPart;
			if (realElapsed > 4.f) {
				isConnected = false;
				logF("Disconnected from injector due to timeout");
				injectorToHorion->isPresent = false;
				QueryPerformanceCounter(&timeSinceLastMessage);
			}
		}
		g_Data.setInjectorConnectionActive(isConnected);

		if (isConnected) {
			// Send Ping every 2 seconds to keep connection alive
			{
				__int64 elapsedPing = endTime.QuadPart - timeSinceLastPing.QuadPart;
				float realPing = (float)elapsedPing / frequency.QuadPart;
				if (realPing > 1) {
					HorionDataPacket pingPacket;
					pingPacket.cmd = CMD_PING;
					pingPacket.params[0] = 0x1333337;
					g_Data.sendPacketToInjector(pingPacket);
					QueryPerformanceCounter(&timeSinceLastPing);
				}
			}

			if (injectorToHorion->isUnread) {  // They sent us a message
				QueryPerformanceCounter(&timeSinceLastMessage);
				switch (injectorToHorion->cmd) {
				case CMD_INIT: {
					logF("Got CMD_INIT from injector");
					loggedIn = true;
					int flags = injectorToHorion->params[0];
					if (flags & (1 << 0) && injectorToHorion->dataSize > 0 && injectorToHorion->dataSize < sizeof(injectorToHorion->data)) {  // Has Json data
						injectorToHorion->data[sizeof(injectorToHorion->data) - 1] = '\0';
						json data = json::parse(reinterpret_cast<char*>(injectorToHorion->data));
						if (data.at("discordAuth").is_string() && data.at("serial").is_number_integer()) {
							logF("Got api token from injector");
							auto serialNum = data.at("serial").get<unsigned int>();
							if (serialNum == 0) {
								logF("Serial is null!");
								GameData::terminate();
							}

							auto roamingFolder = Logger::GetRoamingFolderPath();
							if (roamingFolder.substr(0, 2) == L"C:") {  // Make sure we're getting a handle to the C volume

								HANDLE file = CreateFileW(roamingFolder.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, 0);
								if (file != INVALID_HANDLE_VALUE) {
									unsigned long serial = 0;
									unsigned long maxNameLen = 0, flags2 = 0;
									bool succ = GetVolumeInformationByHandleW(file, 0, 0, &serial, &maxNameLen, &flags2, nullptr, 0);
									if (succ) {
										/*if (serial != serialNum) { // Dont print the raw values here, don't leak serials
											logF("Serial doesn't match! (Diff: %lli)", (long long) serial - (long long)serialNum);
											g_Data.terminate();
										}
										serialNum = serial;*/
									}
									CloseHandle(file);
								}
							}
							g_Data.setAccountInformation(AccountInformation::fromToken(data.at("discordAuth").get<std::string>(), serialNum));
						}
					}
					if (flags & (1 << 2))  // WIP Features
						g_Data.setAllowWIPFeatures(true);

					break;
				}
				case CMD_PING: {
					HorionDataPacket pongPacket;
					pongPacket.cmd = CMD_PONG;
					pongPacket.params[0] = injectorToHorion->params[0];
					g_Data.sendPacketToInjector(pongPacket);
					break;
				}
				case CMD_PONG: {
					break;
				}
				case CMD_RESPONSE: {
					int id = injectorToHorion->params[1];

					auto pk = std::make_shared<HorionDataPacket>();
					pk->cmd = (DATAPACKET_CMD)injectorToHorion->params[0];
					memcpy(pk->params, &injectorToHorion->params[2], sizeof(injectorToHorion->params) - sizeof(int) * 2);

					if (injectorToHorion->dataSize > 0 && injectorToHorion->dataSize < 3000) {
						pk->dataArraySize = injectorToHorion->dataSize;

						auto dataTemp = new unsigned char[injectorToHorion->dataSize + 2];
						memset(dataTemp + injectorToHorion->dataSize, 0, 2);  // If we don't zero the last 2 bytes, printing as unicode string won't work
						memcpy(dataTemp, injectorToHorion->data, injectorToHorion->dataSize);
						auto tmp = std::shared_ptr<unsigned char[]>(dataTemp);
						pk->data.swap(tmp);
					}

					g_Data.callInjectorResponseCallback(id, pk);
					break;
				}
				case CMD_LOG: {
					if(injectorToHorion->dataSize > 4 && injectorToHorion->dataSize < 2999){
						injectorToHorion->data[injectorToHorion->dataSize] = 0; // null terminator

						char* command = reinterpret_cast<char*>(&injectorToHorion->data[3]);
						if(command[1] == cmdMgr->prefix)
							command++;

						cmdMgr->execute(command);
					}
					break;
				}
				default:
					break;
				}

				injectorToHorion->isUnread = false;
			}

			// Send log messages
			{
				auto vecLock = Logger::GetTextToInjectorLock();;

				if (loggedIn && g_Data.isPacketToInjectorQueueEmpty()) {
					auto* stringPrintVector = Logger::GetTextToSend();
#if defined(_DEBUG) or defined(_BETA)
					if (stringPrintVector->size() > 0 && g_Data.isPacketToInjectorQueueEmpty()) {
						auto str = *stringPrintVector->begin();
						stringPrintVector->erase(stringPrintVector->begin());
						
						auto wstr = Utils::stringToWstring(str->text);
						
						const wchar_t* ident = L"log ";
						size_t identLength = wcslen(ident);
						size_t textLength = wcslen(wstr.c_str()) + identLength;

						if(textLength < 2990){
							HorionDataPacket packet;
							packet.cmd = CMD_LOG;
							auto tmp = std::shared_ptr<unsigned char[]>(new unsigned char[(textLength + 1) * sizeof(wchar_t)]);
							packet.data.swap(tmp);
							size_t leng = (textLength + 1) * sizeof(wchar_t);
							wcscpy_s((wchar_t*)packet.data.get(), textLength, ident);
							wcscpy_s((wchar_t*)(packet.data.get() + identLength * sizeof(wchar_t)), textLength - identLength + 1, wstr.c_str());
							packet.dataArraySize = (int)wcslen((wchar_t*)packet.data.get()) * sizeof(wchar_t);

							if(packet.dataArraySize < 2999)
								g_Data.sendPacketToInjector(packet);
						}
					}
#else
					stringPrintVector->clear();
#endif
				}
			}

			if (!horionToInjector->isUnread && !g_Data.isPacketToInjectorQueueEmpty()) {
				QueryPerformanceCounter(&timeSinceLastMessage);
				// They read the message, lets send the next one
				HorionDataPacket nextDataPack = g_Data.getPacketToInjector();
				if (nextDataPack.dataArraySize >= 3000) {
					throw std::exception("Horion Data packet too big to send");
				}

				horionToInjector->cmd = nextDataPack.cmd;
				memcpy(horionToInjector->params, nextDataPack.params, sizeof(int) * 5);
				if (nextDataPack.dataArraySize > 0) {
					memcpy(horionToInjector->data, nextDataPack.data.get(), nextDataPack.dataArraySize);
				}

				horionToInjector->dataSize = nextDataPack.dataArraySize;

				horionToInjector->isUnread = true;
			}
		} else
			Sleep(30);
	}
	logF("Quitting connection thread");
	horionToInjector->isPresent = false;
	memset(magicValues, 0, sizeof(magicValues));
	memset(magicArray, 0, sizeof(magicValues) + sizeof(uintptr_t) * 2);
	Sleep(150);
	delete *horionToInjectorPtr;
	delete *injectorToHorionPtr;
	delete[] magicArray;

	ExitThread(0);
}

#ifndef _MSC_VER
#define _MSC_VER "unk"
#endif

DWORD WINAPI start(LPVOID lpParam) {
	logF("Starting up...");
	logF("MSC v%i at %s", _MSC_VER, __TIMESTAMP__);

	DWORD conThread;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)injectorConnectionThread, lpParam, NULL, &conThread);
	logF("InjCon: %i", conThread);
	init();

	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");  // Get Module for Base Address

	MH_Initialize();
	g_Data.checkGameVersion();
	GameData::initGameData(gameModule, &mem, (HMODULE)lpParam);
	Target::init(g_Data.getPtrLocalPlayer());

	Hooks::Init();

	DWORD keyThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, &keyThreadId);  // Checking Keypresses
	logF("KeyT: %i", keyThreadId);

	logF("Waiting for injector");
	while (!g_Data.isInjectorConnectionActive()) {
		Sleep(10);
		if (!isRunning)
			ExitThread(0);
	}
	logF("Injector found");

	cmdMgr->initCommands();
	logF("Initialized command manager (1/3)");
	moduleMgr->initModules();
	logF("Initialized module manager (2/3)");
	configMgr->init();
	logF("Initialized config manager (3/3)");

	Hooks::Enable();
	TabGui::init();
	ClickGui::init();

	logF("Hooks enabled");

	std::thread countThread([] {
		while (isRunning) {
			Sleep(1000);
			g_Data.fps = g_Data.frameCount;
			g_Data.frameCount = 0;
			g_Data.cpsLeft = g_Data.leftclickCount;
			g_Data.leftclickCount = 0;
			g_Data.cpsRight = g_Data.rightclickCount;
			g_Data.rightclickCount = 0;
		}
	});
	countThread.detach();

	logF("Count thread started");

	ExitThread(0);
}

BOOL __stdcall DllMain(HMODULE hModule,
					   DWORD ul_reason_for_call,
					   LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;
	case DLL_PROCESS_DETACH:
		isRunning = false;

		scriptMgr.unloadAllScripts();
		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();
		//GameWnd.OnDeactivated();

		logF("Removing logger");
		Logger::Disable();

		MH_Uninitialize();
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (g_Data.getLocalPlayer() != nullptr) {
			C_GuiData* guiData = g_Data.getClientInstance()->getGuiData();
			if (guiData != nullptr && !GameData::shouldHide())
				guiData->displayClientMessageF("%sUninjected!", RED);
		}
		break;
	}
	return TRUE;
}
