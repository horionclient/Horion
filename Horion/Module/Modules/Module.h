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
	PLAYER = 3,
	BUILD = 4,
	EXPLOITS = 5
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
	SettingValue* minValue;
	SettingValue* maxValue;

	// ClickGui Data
	bool isDragging = false; // This is incredibly hacky and i wanted to avoid this as much as possible but i want to get this clickgui done

	void makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt() {
		switch (valueType) {
		case TEXT_T:
		case BOOL_T:
			break;
		case INT64_T:
			value->int64 = max(minValue->int64, min(maxValue->int64, value->int64));
			break;
		case DOUBLE_T:
			value->_double = max(minValue->_double, min(maxValue->_double, value->_double));
			break;
		case FLOAT_T:
			value->_float = max(minValue->_float, min(maxValue->_float, value->_float));
			break;
		case INT_T:
			value->_int = max(minValue->_int, min(maxValue->_int, value->_int));
			break;
		default:
			logF("unrecognized value %i", valueType);
		}
	}
};

class IModule
{
private:
	bool enabled = false;
	int keybind = 0x0;
	bool extended = false;
	Category category;

	std::vector<SettingEntry*> settings;
protected:
	IModule(int key, Category c);

	void registerFloatSetting(std::string name, float* floatPtr, float defaultValue, float minValue, float maxValue);
	void registerIntSetting(std::string name, int* intpTr, int defaultValue, int minValue, int maxValue);
	void registerBoolSetting(std::string name, bool* boolPtr, bool defaultValue);
	void registerStringSetting(std::string name, char* textPtr,std::string defaultText);
public:
	~IModule();

	const Category getCategory() { return category; };

	inline std::vector<SettingEntry*>* getSettings() { return &settings; };

	virtual const char* getModuleName() = 0;
	virtual const char* getRawModuleName();
	virtual int getKeybind();
	virtual void setKeybind(int key);
	virtual bool allowAutoStart();

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

