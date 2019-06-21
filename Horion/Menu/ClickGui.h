#pragma once

#include <vector>
#include "../DrawUtils.h"
#include "../Module/ModuleManager.h"



class ClickGui
{
private:


	static void renderLabel(const char* text);
public:
	static void init();
	static void render();
	static void onMouseClickUpdate(bool isDown);
};
void getModuleListByCategory(Category category);