#pragma once

#include <vector>
#include <map>
#include "../DrawUtils.h"
#include "../GuiUtils.h"
#include "../../Memory/GameData.h"
#include "../Module/ModuleManager.h"


struct ClickWindow {
	ClickWindow() {
		pos.x = 500;
		pos.y = 250;
	}
	vec2_t pos;
	bool isFocused = false;
	bool isExtended = true;
};

class ClickGui
{
private:
	static std::shared_ptr<ClickWindow> getWindow(const char* id);
	

	static void renderCategory(Category category);
	static void renderSettings(IModule * mod);
	static void getModuleListByCategory(Category category, std::vector<IModule*>* modList);
	static void getExtendedModuleList(bool isExtended, std::vector<IModule*>* modList);
	static unsigned int getCrcHash(const char* str);
	static unsigned int getWindowHash(const char * name);
public:
	static void init();
	static void render();
	static void onMouseClickUpdate(int key, bool isDown);
};