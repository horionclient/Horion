#include "Module.h"



IModule::IModule(int key, Category c)
{
	this->keybind = key;
	this->category = c;
	this->registerIntSetting(std::string("keybind"), &this->keybind, this->keybind, 0, 0xFF);
	this->registerBoolSetting(std::string("enabled"), &this->enabled, false);
}

void IModule::registerFloatSetting(std::string name, float* floatPtr, float defaultValue, float minValue, float maxValue)
{
#ifdef DEBUG
	if (minValue > maxValue)
		__debugbreak(); // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	setting->valueType = FLOAT_T;
	
	setting->value = reinterpret_cast<SettingValue*>(floatPtr);

	// Default Value
	SettingValue* defaultVal = new SettingValue(); 
	defaultVal->_float = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_float = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_float = maxValue;
	setting->maxValue = maxVal;

	strcpy_s(setting->name, 19, name.c_str()); // Name

	settings.push_back(setting); // Add to list
}

void IModule::registerIntSetting(std::string name, int * intPtr, int defaultValue, int minValue, int maxValue)
{
#ifdef DEBUG
	if (minValue > maxValue)
		__debugbreak(); // Minimum value is bigger than maximum value
#endif

	SettingEntry* setting = new SettingEntry();
	setting->valueType = INT_T;
	setting->value = reinterpret_cast<SettingValue*>(intPtr); // Actual Value

	// Default Value
	SettingValue* defaultVal = new SettingValue(); 
	defaultVal->_int = defaultValue;
	setting->defaultValue = defaultVal;

	// Min Value
	SettingValue* minVal = new SettingValue();
	minVal->_int = minValue;
	setting->minValue = minVal;

	// Max Value
	SettingValue* maxVal = new SettingValue();
	maxVal->_int = maxValue;
	setting->maxValue = maxVal;

	// Name
	strcpy_s(setting->name, 19, name.c_str()); 

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

void IModule::registerStringSetting(std::string name, char* textPtr, std::string defaultText)
{
	SettingEntry* setting = new SettingEntry();
	setting->valueType = TEXT_T;

	setting->value = reinterpret_cast<SettingValue*>(textPtr); // Actual value

	SettingValue* defaultVal = new SettingValue(); // Default Value
	defaultVal->text = &defaultText;
	setting->defaultValue = defaultVal;

	strcpy_s(setting->name, 19, name.c_str()); // Name

	settings.push_back(setting); // Add to list
}

IModule::~IModule()
{
}

const char* IModule::getRawModuleName()
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

bool IModule::allowAutoStart()
{
	return true;
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
						break;
					case DOUBLE_T:
						sett->value->_double = value.get<double>();
						break;
					case INT64_T:
						sett->value->int64 = value.get<__int64>();
						break;
					case INT_T:
						sett->value->_int = value.get<int>();
						break;
					case BOOL_T:
						sett->value->_bool = value.get<bool>();
						break;
					case TEXT_T:
						sett->value->text = &value.get<std::string>();
						break;
					}
					sett->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
					continue;
				}
				catch (std::exception e) {
					logF("Config Load Error (%s): %s", this->getRawModuleName(), e.what());
				}
			}
		}
		if (this->enabled)
			this->onEnable();
	}
}

#pragma warning( push )
#pragma warning( disable : 26444 )
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

#pragma warning( pop ) 

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
			logF("%s %s", enabled ? "Enabled" : "Disabled", this->getModuleName());

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