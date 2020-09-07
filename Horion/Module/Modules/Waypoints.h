#pragma once

#include <sstream>
#include "../../DrawUtils.h"
#include "Module.h"
#include "../../../Utils/Json.hpp"

class Waypoints : public IModule {
private:
	std::map<std::string, vec3_t> waypoints;

public:
	Waypoints();
	~Waypoints();

	float size = 0.6f;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	virtual void onLoadConfig(void* confVoid) override;
	virtual void onSaveConfig(void* confVoid) override;

	bool add(std::string text, vec3_t pos) {
		for (const auto& _wp : waypoints) {
			if (text == _wp.first) {
				return false;
			}
		}
		waypoints[text] = pos;
		return true;
	}

	bool remove(std::string name) {
		for (const auto& _wp : waypoints) {
			if (name == _wp.first) {
				waypoints.erase(name);
				return true;
			}
		}
		return false;
	}
};