#pragma once

#include <vector>
#include <map>
#include "../DrawUtils.h"
#include "../GuiUtils.h"
#include "../../Memory/GameData.h"
#include "../Module/ModuleManager.h"

struct ClickModule {
	bool isExtended = false;
};

struct ClickWindow {
	ClickWindow() {
		pos.x = 500;
		pos.y = 250;

		size.x = 30;
		size.y = 30;
	}
	vec2_t pos;
	vec2_t size;
	bool isExtended = true;
	bool isInAnimation = false;
	float animation = 0;
	std::map<unsigned int, std::shared_ptr<ClickModule>> moduleMap;
};

class ClickGui
{
private:
	inline static std::shared_ptr<ClickWindow> getWindow(const char* id);
	inline static std::shared_ptr<ClickModule> getClickModule(std::shared_ptr<ClickWindow> window, const char* id);

	static void renderLabel(const char* text);
	static void renderTooltip(std::string* text, vec2_t mousepos);

	static void renderCategory(Category category);
	inline static void getModuleListByCategory(Category category, std::vector<IModule*>* modList);
	inline static unsigned int getCrcHash(const char* str, int seed = 0);
public:
	static void init();
	static void render();
	static void onKeyUpdate(int key, bool isDown);
	static void onMouseClickUpdate(int key, bool isDown);
};