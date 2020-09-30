#include "Waypoints.h"

Waypoints::Waypoints() : IModule(0x0, Category::VISUAL, "Shows holograms for user-defined coordinates") {
	registerFloatSetting("Size", &size, size, 0.3, 1.6);
}

Waypoints::~Waypoints() {
}

const char* Waypoints::getModuleName() {
	return "Waypoints";
}

void Waypoints::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();

	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		for (auto it = waypoints->begin(); it != waypoints->end(); it++) {
			vec3_t pos = it->second;
			float dist = pos.dist(*g_Data.getLocalPlayer()->getPos());

			std::ostringstream out;
			out.precision(2);
			out << it->first << " (" << std::fixed << dist << " m)";

			DrawUtils::drawHologram(pos.add(0, 1.68, 0), out.str(), fmax(size, 3.f / dist));
			DrawUtils::flush();
		}
	}
}

using json = nlohmann::json;
void Waypoints::onLoadConfig(void* confVoid) {
	IModule::onLoadConfig(confVoid);  // retain keybinds & enabled state
	waypoints->clear();
	json* conf = reinterpret_cast<json*>(confVoid);
	std::string modName = getRawModuleName();
	if (conf->contains(modName.c_str())) {
		auto obj = conf->at(modName.c_str());
		if (obj.is_null())
			return;
		if (obj.contains("list")) {
			auto value = obj.at("list");
			if (value.is_null() || !value.is_object())
				return;

			for (json::iterator it = value.begin(); it != value.end(); ++it) {
				vec3_t _pos;
				auto val = it.value();
				if (!val.contains("pos")) 
					continue;
				auto pos = val.at("pos");
				if (!pos.is_null() && pos.contains("x") && pos["x"].is_number_float() && pos.contains("y") && pos["y"].is_number_float() && pos.contains("z") && pos["z"].is_number_float()) {
					try {
						_pos = vec3_t(pos["x"].get<double>(), pos["y"].get<double>(), pos["z"].get<double>());
					} catch (std::exception e) {
					}
				} else {
					continue;
				}
				waypoints->emplace(it.key().c_str(), _pos);
			}
		}
	}
}

void Waypoints::onSaveConfig(void* confVoid) {
	IModule::onSaveConfig(confVoid);  // retain keybinds & enabled state
	json* conf = reinterpret_cast<json*>(confVoid);

	std::string modName = getRawModuleName();
	json obj = {};

	if (conf->contains(modName.c_str())) {
		obj = conf->at(modName.c_str());
		conf->erase(modName.c_str());
	}
	if (obj.contains("list")) {
		obj.erase("list");
	}

	json myList = {};

	for (auto it = waypoints->begin(); it != waypoints->end(); it++) {
		json subObj = {};
		subObj["pos"]["x"] = (float)it->second.x;
		subObj["pos"]["y"] = (float)it->second.y;
		subObj["pos"]["z"] = (float)it->second.z;
		myList.emplace(it->first.c_str(), subObj);
	}

	if (waypoints->size() > 0) {
		obj.emplace("list", myList);
	}

	conf->emplace(modName.c_str(), obj);
}
