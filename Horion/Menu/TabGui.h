#pragma once

#include <vector>

#include "../DrawUtils.h"
#include "../Module/ModuleManager.h"

class TabGui {
private:
	static void renderLabel(const char* text, std::shared_ptr<IModule> mod = 0);
	static void renderLevel();

public:
	static void init();
	static void render();
	static void onKeyUpdate(int key, bool isDown);
};
