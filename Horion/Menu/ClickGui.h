#pragma once

#include <vector>
#include "../DrawUtils.h"
#include "../GuiUtils.h"
#include "../Module/ModuleManager.h"

class ClickGui
{
private:
	static void renderLabel(const char* text);
	static void getModuleListByCategory(Category category);
public:
	static void init();
	static void render();
	static void onMouseClickUpdate(bool isDown);
};