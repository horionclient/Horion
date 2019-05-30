#include "Module.h"



IModule::IModule(int key, Category c)
{
	this->keybind = key;
	this->category = c;
	this->registerIntSetting(std::string("keybind"), &this->keybind, this->keybind);
	this->registerBoolSetting(std::string("enabled"), &this->enabled, false);
}

void IModule::registerFloatSetting(std::string name, float* floatPtr, float defaultValue)
{
	SettingEntry* setting = new SettingEntry();
	setting->valueType = FLOAT_T;
	
	setting->value = reinterpret_cast<SettingValue*>(floatPtr);

	SettingValue* defaultVal = new SettingValue(); // Default Value
	defaultVal->_float = defaultValue;
	setting->defaultValue = defaultVal;

	strcpy_s(setting->name, 19, name.c_str()); // Name

	settings.push_back(setting); // Add to list
}

void IModule::registerIntSetting(std::string name, int * intPtr, int defaultValue)
{
	SettingEntry* setting = new SettingEntry();
	setting->valueType = INT_T;

	setting->value = reinterpret_cast<SettingValue*>(intPtr); // Actual value

	SettingValue* defaultVal = new SettingValue(); // Default Value
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	strcpy_s(setting->name, 19, name.c_str()); // Name

	settings.push_back(setting); // Add to list
}

void IModule::registerBoolSetting(std::string name, bool * boolPtr, bool defaultValue)
{
	SettingEntry* setting = new SettingEntry();
	setting->valueType = BOOL_T;

	setting->value = reinterpret_cast<SettingValue*>(boolPtr); // Actual value

	SettingValue* defaultVal = new SettingValue(); // Default Value
	defaultVal->_bool = defaultValue;
	setting->defaultValue = defaultVal;

	strcpy_s(setting->name, 19, name.c_str()); // Name

	settings.push_back(setting); // Add to list
}

IModule::~IModule()
{
}

std::string IModule::getRawModuleName()
{
	return getModuleName();
}

int IModule::getKeybind()
{
	return this->keybind;
}

void IModule::setKeybind(int key) {
	this->keybind = key;
}

void IModule::onTick(C_GameMode*)
{
}

void IModule::onKeyUpdate(int key, bool isDown)
{
	if (key == getKeybind()) {
		if (isFlashMode())
			setEnabled(isDown);
		else if (isDown)
			toggle();
	}
		
}

void IModule::onEnable()
{
}

void IModule::onDisable()
{
}

void IModule::onPreRender()
{
}

void IModule::onPostRender()
{
}

void IModule::onLoadConfig(json * conf)
{
	if (conf->contains(this->getRawModuleName())) {
		auto obj = conf->at(this->getRawModuleName());
		if (obj.is_null())
			return;
		for (auto it = this->settings.begin(); it != this->settings.end(); ++it) {
			SettingEntry* sett = *it;
			if (obj.contains(sett->name)) {
				auto value = obj.at(sett->name);
				if (value.is_null())
					continue;
				try {
					switch (sett->valueType) {
					case FLOAT_T:
						sett->value->_float = value.get<float>();
						continue;
					case DOUBLE_T:
						sett->value->_double = value.get<double>();
						continue;
					case INT64_T:
						sett->value->int64 = value.get<__int64>();
						continue;
					case INT_T:
						sett->value->_int = value.get<int>();
						continue;
					case BOOL_T:
						sett->value->_bool = value.get<bool>();
						continue;
					case TEXT_T:
						sett->value->text = &value.get<std::string>();
						continue;
					}
				}
				catch (std::exception e) {
					logF("Config Load Error (%s): %s", this->getRawModuleName().c_str(), e.what());
				}
				
			}
		}
	}
	
}

void IModule::onSaveConfig(json * conf)
{
	std::string modName = getRawModuleName();
	if (conf->contains(modName.c_str()))
		conf->erase(modName.c_str());
	
	json obj = {};
	//auto obj = conf->at(modName);
	for (auto it = this->settings.begin(); it != this->settings.end(); ++it) {
		SettingEntry* sett = *it;
		switch (sett->valueType) {
		case FLOAT_T:
			obj.emplace(sett->name, sett->value->_float);
			break;
		case DOUBLE_T:
			obj.emplace(sett->name, sett->value->_double);
			break;
		case INT64_T:
			obj.emplace(sett->name, sett->value->int64);
			break;
		case INT_T:
			obj.emplace(sett->name, sett->value->_int);
			break;
		case BOOL_T:
			obj.emplace(sett->name, sett->value->_bool);
			break;
		case TEXT_T:
			obj.emplace(sett->name, *sett->value->text);
			break;
		}
	}
	
	conf->emplace(modName.c_str(), obj);
}

bool IModule::isFlashMode()
{
	return false;
}


void IModule::setEnabled(bool enabled)
{
	if (this->enabled != enabled) {
		this->enabled = enabled;
#ifndef _DEBUG
		if(!isFlashMode()) // Only print jetpack stuff in debug mode
#endif
			logF("%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName().c_str());

		if (enabled)
			this->onEnable();
		else
			this->onDisable();
	}
}

void IModule::toggle()
{
	setEnabled(!this->enabled);
}

bool IModule::isEnabled()
{
	return this->enabled;
}
