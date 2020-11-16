#include "ConfigManager.h"

#include <windows.storage.h>
#include <wrl.h>

using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

std::wstring ConfigManager::GetRoamingFolderPath() {
	ComPtr<IApplicationDataStatics> appDataStatics;
	auto hr = RoGetActivationFactory(HStringReference(L"Windows.Storage.ApplicationData").Get(), __uuidof(appDataStatics), &appDataStatics);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve application data statics");

	ComPtr<IApplicationData> appData;
	hr = appDataStatics->get_Current(&appData);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve current application data");

	ComPtr<IStorageFolder> roamingFolder;
	hr = appData->get_RoamingFolder(&roamingFolder);
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder");

	ComPtr<IStorageItem> folderItem;
	hr = roamingFolder.As(&folderItem);
	if (FAILED(hr)) throw std::runtime_error("Failed to cast roaming folder to IStorageItem");

	HString roamingPathHString;
	hr = folderItem->get_Path(roamingPathHString.GetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed to retrieve roaming folder path");

	uint32_t pathLength;
	auto roamingPathCStr = roamingPathHString.GetRawBuffer(&pathLength);
	return std::wstring(roamingPathCStr, pathLength);
}

ConfigManager::ConfigManager() {
	this->roamingFolder = GetRoamingFolderPath();
}

ConfigManager::~ConfigManager() {
}

void ConfigManager::loadConfig(std::string name, bool create) {
	
	size_t allocSize = name.size() + roamingFolder.size() + 20;  // std::wstring::size() can be wierd so lets make sure this actually fits
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.h", roamingFolder.c_str(), name.c_str());

	const bool configExists = std::filesystem::exists(fullPath);
	if (configExists || create) {
		if (name != currentConfig)
			saveConfig();  // Save old config

		currentConfig = name;
		if (configExists) {
			std::ifstream confFile(fullPath, std::ifstream::binary);
			json conf;
			try {
				currentConfigObj.clear();
				confFile >> currentConfigObj;
			} catch (json::parse_error& e) {
				logF("Config Load Exception!: %s", e.what());
			}
			currentConfigObj["from"] = "Horion";
		}

		if (configExists) {
			moduleMgr->onLoadConfig(&currentConfigObj);
			if (currentConfigObj.contains("prefix")) {
				std::string prefix = currentConfigObj["prefix"];
				cmdMgr->prefix = prefix.at(0);
			}
		}

		if (create) 
			saveConfig();

		if (g_Data.getLocalPlayer() != nullptr) {
			static bool helpedUser = false;
			g_Data.getGuiData()->displayClientMessageF("[%sHorion%s] %sSuccessfully %s config %s%s%s!", GOLD, WHITE, GREEN, !configExists ? "created" : "loaded", GRAY, name.c_str(), GREEN);
			if (!helpedUser && name != "default") {
				helpedUser = true;
				g_Data.getGuiData()->displayClientMessageF("[%sHorion%s] %sEnter \"%s%cconfig load default%s\" to load your old config!", GOLD, WHITE, YELLOW, WHITE, cmdMgr->prefix, YELLOW);
			}
		}
	} else {
		if (g_Data.getLocalPlayer() != nullptr) 
			g_Data.getGuiData()->displayClientMessageF("[%sHorion%s] %sCould not load config %s%s%s!", GOLD, WHITE, RED, GRAY, name.c_str(), RED);
	}

	delete[] fullPath;
}

void ConfigManager::saveConfig() {
	logF("Saving config %s", currentConfig.c_str());
	size_t allocSize = currentConfig.size() + roamingFolder.size() + 20;  // std::wstring::size() can be wierd so lets make sure this actually fits
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.h", roamingFolder.c_str(), currentConfig.c_str());

	moduleMgr->onSaveConfig(&currentConfigObj);

	std::string prefix;
	prefix.push_back(cmdMgr->prefix);
	currentConfigObj["prefix"] = prefix;

	std::ofstream o(fullPath, std::ifstream::binary);
	o << std::setw(4) << currentConfigObj << std::endl;
	o.flush();
	o.close();

	delete[] fullPath;
}

void ConfigManager::init() {
	logF("Initializing config");
	loadConfig(currentConfig, true);
}

ConfigManager* configMgr = new ConfigManager();
