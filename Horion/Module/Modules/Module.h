#pragma once

#include "../../../Memory/GameData.h"
#include "../../../Utils/Json.hpp"
#include <string>

using json = nlohmann::json;

enum ValueType {
	FLOAT_T,
	DOUBLE_T,
	INT64_T,
	INT_T,
	TEXT_T
};

struct SettingValue {
	union {
		float _float;
		double _double;
		__int64 int64;
		int _int;
		std::string* text;
	};
};

struct SettingEntry {
	char name[0x20];
	ValueType valueType;
	SettingValue* value;
	SettingValue* defaultValue;
};

class IModule
{
private:
	bool enabled = false;
	int keybind = 0x0;
	char rawModuleName[30];

	std::vector<SettingEntry*> settings;
protected:
	IModule(int key);

	void registerFloatSetting(std::string name, float* floatPtr, float defaultValue);

public:
	~IModule();

	virtual std::string getModuleName() = 0;
	virtual std::string getRawModuleName();
	virtual int getKeybind();
	virtual void setKeybind(int key);

	virtual void onTick(C_GameMode*);
	virtual void onKeyUpdate(int key, bool isDown);
	virtual void onEnable();
	virtual void onDisable();
	virtual void onPreRender();
	virtual void onPostRender();
	virtual void onLoadConfig(json* conf);
	virtual void onSaveConfig(json* conf);
	virtual bool isFlashMode();
	virtual void setEnabled(bool enabled);
	virtual void toggle();
	virtual bool isEnabled();
};

