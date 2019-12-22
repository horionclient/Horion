#include "ConfigManager.h"



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
	size_t allocSize = name.size() + roamingFolder.size() + 20; // std::wstring::size() can be wierd so lets make sure this actually fits
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.h", roamingFolder.c_str(), name.c_str());

	const bool configExists = std::filesystem::exists(fullPath);
	if (configExists || create) {
		currentConfig = name;
		if (configExists) {
			std::ifstream confFile(fullPath, std::ifstream::binary);
			json conf;
			try {
				currentConfigObj.clear();
				confFile >> currentConfigObj;
			}
			catch (json::parse_error& e) {
				logF("Config Load Exception!: %s", e.what());
			}
			currentConfigObj["from"] = "Horion";
		}

		moduleMgr->onLoadConfig(&currentConfigObj);

		if (create) {
			saveConfig();
		}
	}

	delete[] fullPath;
}

void ConfigManager::saveConfig() {
	size_t allocSize = currentConfig.size() + roamingFolder.size() + 20; // std::wstring::size() can be wierd so lets make sure this actually fits
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.h", roamingFolder.c_str(), currentConfig.c_str());

	moduleMgr->onSaveConfig(&currentConfigObj);

	std::ofstream o(fullPath, std::ifstream::binary);
	o << std::setw(4) << currentConfigObj << std::endl;
	o.flush();
	o.close();

	delete[] fullPath;
}

void ConfigManager::init() {
	loadConfig(currentConfig, true);
}

ConfigManager* configMgr = new ConfigManager();