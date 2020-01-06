#include "Loader.h"

SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif

DWORD WINAPI analyticsThread(LPVOID lpParam) {
	logF("Analytics started");
	__try {
		auto sendRequest = [](const char* request) {
#ifndef _DEBUG
			wchar_t fullUrl[200];
			swprintf_s(fullUrl, 200, L"https://hbob.ml/horion/action.php?type=%S", request);
			WinHttpClient client(fullUrl);

			// Send HTTP request, a GET request by default.
			client.SendHttpRequest();

			// The response header.
			std::wstring httpResponseHeader = client.GetResponseHeader();
#endif
		};

		sendRequest("startup");

		LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;

		QueryPerformanceFrequency(&Frequency);
		QueryPerformanceCounter(&StartingTime);

		while (isRunning) {
			{
				QueryPerformanceCounter(&EndingTime);
				ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
				ElapsedMicroseconds.QuadPart *= 1000;
				ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
				if (ElapsedMicroseconds.QuadPart < 1000 * 60 * 2) {
					Sleep(50);
					continue;
				} else
					QueryPerformanceCounter(&StartingTime);
			}
			char url[200];
			char serverIp[200];
			if (g_Data.getRakNetInstance() != nullptr && g_Data.getRakNetInstance()->serverIp.getTextLength() >= 0)
				strcpy_s(serverIp, g_Data.getRakNetInstance()->serverIp.getText());
			sprintf_s(url, 200, "continuous&s=%s", serverIp[0] == 0 ? "none" : serverIp);
			sendRequest(url);
		}
	} __except (EXCEPTION_EXECUTE_HANDLER) {
#ifdef _DEBUG
		logF("Analytics Thread crashed!");
		__debugbreak();
#else
		ExitThread(0);
#endif
	}

	logF("Analytics thread exitted");

	ExitThread(0);
}

DWORD WINAPI keyThread(LPVOID lpParam) {
	logF("Key thread started");

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if (keyMap == 0)
		throw std::exception("Keymap not allocated");

	uintptr_t clickMap = reinterpret_cast<uintptr_t>(malloc(5));
	if (clickMap == 0)
		throw std::exception("Clickmap not allocated");

	bool* keyMapAddr = 0x0;
	uintptr_t sigOffset = Utils::FindSignature("48 8D 0D ?? ?? ?? ?? 89 1C 81 48");
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

		C_RakNetInstance* rakInstance = g_Data.getRakNetInstance();
		if (rakInstance != nullptr && rakInstance->serverIp.getTextLength() > 5 &&
			(strcmp(rakInstance->serverIp.getText(), "play.valeanetwork.eu") == 0 ||
			 strcmp(rakInstance->serverIp.getText(), "137.74.152.142") == 0 ||
			 strcmp(rakInstance->serverIp.getText(), "pvp.valeanetwork.eu") == 0)) {
			C_GuiData* guiData = g_Data.getClientInstance()->getGuiData();

			if (guiData != nullptr) guiData->displayClientMessageF("%sHorion Client is not allowed in Valea Network.", RED);

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

		if (*hidController != 0) {
			for (uintptr_t key = 0; key < 5; key++) {
				bool newKey = (*hidController)->clickMap[key];
				bool* oldKey = reinterpret_cast<bool*>(clickMap + key);
				if (newKey != *oldKey) {
					ClickGui::onMouseClickUpdate((int)key, newKey);
				}
			}

			memcpy(reinterpret_cast<void*>(clickMap), &(*hidController)->leftClickDown, 5);
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);

		Sleep(2);
	}
	logF("Aight bro I'm boutta head out");
	Sleep(150);  // Give the threads a bit of time to exit

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

	MemoryBoi** horionToInjectorPtr = reinterpret_cast<MemoryBoi**>(magicArray + sizeof(magicValues));
	MemoryBoi** injectorToHorionPtr = reinterpret_cast<MemoryBoi**>(magicArray + sizeof(magicValues) + sizeof(uintptr_t));

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

	while (isRunning) {
		Sleep(5);
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
					int flags = injectorToHorion->params[0];
					if (flags & (1 << 0) && injectorToHorion->dataSize > 0 && injectorToHorion->dataSize < sizeof(injectorToHorion->data)) {  // Has Json data
						injectorToHorion->data[sizeof(injectorToHorion->data) - 1] = '\0';
						json data = json::parse(reinterpret_cast<char*>(injectorToHorion->data));
						if (data.at("discordAuth").is_string() && data.at("serial").is_number_integer()) {
							logF("Got discord auth token from injector");
							g_Data.setAccountInformation(AccountInformation::fromToken(data.at("discordAuth").get<std::string>(), data.at("serial").get<unsigned int>()));
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
				default:
					break;
				}

				injectorToHorion->isUnread = false;
			}

			if (!horionToInjector->isUnread && !g_Data.isPacketToInjectorQueueEmpty()) {
				QueryPerformanceCounter(&timeSinceLastMessage);
				// They read the message, lets send the next one
				HorionDataPacket nextDataPack = g_Data.getPacketToInjector();
				if (nextDataPack.dataArraySize >= 3000) {
					delete[] nextDataPack.data;
					throw std::exception("Horion Data packet too big to send");
				}

				horionToInjector->cmd = nextDataPack.cmd;
				memcpy(horionToInjector->params, nextDataPack.params, sizeof(int) * 5);
				if (nextDataPack.dataArraySize > 0) {
					memcpy(horionToInjector->data, nextDataPack.data, nextDataPack.dataArraySize);
					delete[] nextDataPack.data;
				}

				horionToInjector->dataSize = nextDataPack.dataArraySize;

				horionToInjector->isUnread = true;
			}
		} else
			Sleep(30);
	}
	logF("Quitting connection thread");
	memset(magicValues, 0, sizeof(magicValues));
	memset(magicArray, 0, sizeof(magicValues) + sizeof(uintptr_t) * 2);
	delete *horionToInjectorPtr;
	delete *injectorToHorionPtr;
	delete[] magicArray;

	ExitThread(0);
}

DWORD WINAPI startCheat(LPVOID lpParam) {
	logF("Starting up...");
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)injectorConnectionThread, lpParam, NULL, NULL);
	init();

	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");  // Get Module for Base Address

	MH_Initialize();
	GameData::initGameData(gameModule, &mem, (HMODULE)lpParam);
	Target::init(g_Data.getPtrLocalPlayer());

	TabGui::init();
	ClickGui::init();
	Hooks::Init();

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, NULL);  // Checking Keypresses

	logF("Waiting for injector");
	while (!g_Data.isInjectorConnectionActive()) {
		Sleep(10);
		if (!isRunning)
			ExitThread(0);
	}
	logF("Injector found");

	cmdMgr->initCommands();
	moduleMgr->initModules();
	configMgr->init();

	logF("Starting threads...");

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)analyticsThread, lpParam, NULL, NULL);

	ExitThread(0);
}

BOOL __stdcall DllMain(HMODULE hModule,
					   DWORD ul_reason_for_call,
					   LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)startCheat, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;
	case DLL_PROCESS_DETACH:
		isRunning = false;

		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();

		logF("Removing logger");
		Logger::Disable();

		MH_Uninitialize();
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (g_Data.getClientInstance()->getLocalPlayer() != nullptr) {
			C_GuiData* guiData = g_Data.getClientInstance()->getGuiData();
			if (guiData != nullptr)
				guiData->displayClientMessageF("%sUninjected!", RED);
		}
		break;
	}
	return TRUE;
}
