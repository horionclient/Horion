#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <filesystem>
#include <string>
#include <fstream>
#include <windows.storage.h>
#include <wrl.h>
#pragma comment(lib,"runtimeobject")

#include "../Module/ModuleManager.h"
#include "../Command/CommandMgr.h"
#include "../../Utils/Json.hpp"

using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;
using json = nlohmann::json;

class ConfigManager
{
private:
	std::string currentConfig = "default";
	std::wstring roamingFolder;
	static std::wstring GetRoamingFolderPath();
	json currentConfigObj;
public:
	ConfigManager();
	~ConfigManager();

	void loadConfig(std::string name, bool create);
	void saveConfig();
	void init();
};

extern ConfigManager* configMgr;