#include "Waypoints.h"

#include <algorithm>

Waypoints::Waypoints() : IModule(0x0, Category::VISUAL, "Shows holograms for user-defined coordinates") {
	registerFloatSetting("Size", &size, size, 0.3f, 1.6f);
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

			constexpr bool useFloatingPoint = false;
			constexpr bool fadeOutAtDistance = true;
			
			std::string txt;
			if (useFloatingPoint) {
				std::ostringstream out;
				out.precision(2);
				out << it->first << " (" << std::fixed << dist << " m)";
				txt = out.str();
			} else {
				txt = it->first + " (" + std::to_string(int(dist)) + "m)";
			}

			float alpha = 1;

			if (fadeOutAtDistance && dist > 15) {
				
				vec2_t angle = localPlayer->currentPos.CalcAngle(pos);
				float diff = angle.sub(localPlayer->viewAngles).normAngles().magnitude();
				if (dist > 30) {
					float neededDiff = lerp(40, 15, std::min((dist - 30) / 300, 1.f));
					float minAlpha = lerp(0.6f, 0.3f, std::min((dist - 30) / 50, 1.f));
					if (diff < neededDiff)
						alpha = 1.f;
					else if (diff > neededDiff + 10)
						alpha = minAlpha;
					else
						alpha = lerp(1.f, minAlpha, (diff - neededDiff) / 10);
				}
			}

			if (alpha < 0.01f)
				continue;

			vec4_t rectPos;

			txt = Utils::sanitize(txt);

			float textWidth = DrawUtils::getTextWidth(&txt, size);
			float textHeight = DrawUtils::getFont(Fonts::RUNE)->getLineHeight() * size;

			vec2_t textPos = DrawUtils::worldToScreen(pos);
			if (textPos.x != -1) {
				textPos.y -= textHeight;
				textPos.x -= textWidth / 2.f;
				rectPos.x = textPos.x - 1.f * size;
				rectPos.y = textPos.y - 1.f * size;
				rectPos.z = textPos.x + textWidth + 1.f * size;
				rectPos.w = textPos.y + textHeight + 2.f * size;

				DrawUtils::fillRectangle(rectPos, MC_Color(0, 0, 0), alpha * 0.5f);
				DrawUtils::drawText(textPos, &txt, MC_Color(255, 255, 255), size, alpha);
			}
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
