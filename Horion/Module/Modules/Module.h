#pragma once

#include "../../../Memory/GameData.h"
#include "../../../Utils/Json.hpp"
#include "../../FriendList/FriendList.h"
#include <string>

using json = nlohmann::json;

enum Category {
	COMBAT = 0,
	VISUAL = 1,
	MOVEMENT = 2,
	BUILD = 3,
	EXPLOITS = 4
};

enum ValueType {
	FLOAT_T,
	DOUBLE_T,
	INT64_T,
	INT_T,
	BOOL_T,
	TEXT_T
};

struct SettingValue {
	union {
		float _float;
		double _double;
		__int64 int64;
		int _int;
		bool _bool;
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
	Category category;

	std::vector<SettingEntry*> settings;
protected:
	IModule(int key, Category c);

	void registerFloatSetting(std::string name, float* floatPtr, float defaultValue);
	void registerIntSetting(std::string name, int* intpTr, int defaultValue);
	void registerBoolSetting(std::string name, bool* boolPtr, bool defaultValue);
public:
	~IModule();

	const Category getCategory() { return category; };

	virtual const char* getModuleName() = 0;
	virtual const char* getRawModuleName();
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

