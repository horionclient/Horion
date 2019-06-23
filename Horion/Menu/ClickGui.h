#pragma once

#include <vector>
#include <map>
#include "../DrawUtils.h"
#include "../GuiUtils.h"
#include "../Module/ModuleManager.h"

struct ClickWindow {
	vec2_t pos;
};


class ClickGui
{
private:
	static std::shared_ptr<ClickWindow> getWindow(const char* id);
	

	static void renderCategory(Category category);
	static void getModuleListByCategory(Category category, std::vector<IModule*>* modList);
	static unsigned int getCrcHash(const char* str);
public:
	static void init();
	static void render();
	static void onMouseClickUpdate(int key, bool isDown);
};