#include "Waypoints.h"

#include <algorithm>

Waypoints::Waypoints() : IModule(0, Category::VISUAL, "Shows holograms for user-defined coordinates") {
	registerFloatSetting("Size", &size, size, 0.3f, 1.6f);
	registerBoolSetting("Interdimensional", &interdimensional, interdimensional);
	registerBoolSetting("Show coordinates", &showCoordinates, showCoordinates);
}

Waypoints::~Waypoints() {
}

const char* Waypoints::getModuleName() {
	return "Waypoints";
}

void Waypoints::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr || !GameData::canUseMoveKeys())
		return;
	int currentDimension = -1;

	localPlayer->getDimensionId(&currentDimension);

	for (auto it = waypoints->begin(); it != waypoints->end(); it++) {
		vec3_t pos = it->second.pos;
		int wpDimension = it->second.dimension;
		if (!interdimensional && currentDimension != wpDimension)
			continue;
		if (currentDimension == 1 && wpDimension == 0) {
			// we are in nether, wp in overworld
			pos.x /= 8;
			pos.z /= 8;
		} else if (currentDimension == 0 && wpDimension == 1) {
			// we are in overworld, wp in nether
			pos.x *= 8;
			pos.z *= 8;
		} else if (currentDimension != wpDimension)
			continue;
		float dist = pos.dist(*g_Data.getLocalPlayer()->getPos());

		constexpr bool useFloatingPoint = false;
		constexpr bool fadeOutAtDistance = true;
			
		std::string txt;
		if (useFloatingPoint) {
			std::ostringstream out;
			out.precision(2);
			out << it->first << " (" << std::fixed << dist << " m";
			txt = out.str();
		} else {
			txt = it->first + " (" + std::to_string(int(dist)) + "m";
		}
		if (currentDimension != wpDimension) {
			if (wpDimension == 0)
				txt = txt + ", overworld)";
			else if (wpDimension == 1)
				txt = txt + ", nether)";
		}else
			txt = txt + ")";

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

		float textWidth = DrawUtils::getTextWidth(&txt, size) + 0.5f;
		float textHeight = DrawUtils::getFont(Fonts::RUNE)->getLineHeight() * size;

		vec2_t textPos = DrawUtils::worldToScreen(pos);
		if (textPos.x != -1) {
			std::string coordText;
			textPos.y -= textHeight;
			textPos.x -= textWidth / 2.f;

			if (this->showCoordinates) {
				std::ostringstream out;
				out.precision(1);
				out << "(" << std::fixed << pos.x << ", " << pos.y << ", " << pos.z << ")";
				coordText = out.str();
				textWidth = std::max(textWidth, DrawUtils::getTextWidth(&coordText, size * 0.75f) + 0.5f);
			}

			rectPos.x = textPos.x - 1.f * size;
			rectPos.y = textPos.y - 1.05f * size;
			rectPos.z = textPos.x + textWidth + 1.f * size;
			rectPos.w = textPos.y + textHeight + 2.f * size;

			if (this->showCoordinates) 
				rectPos.w += textHeight * 0.75f + 1.f * size;

			MC_Color color(0, 0, 0);
			if (currentDimension == 0 && wpDimension == 1) 
				color.r = 0.2f;
			
			if (currentDimension == 1 && wpDimension == 0) 
				color.b = 0.2f;

			DrawUtils::fillRectangle(rectPos, color, alpha * 0.5f);
			DrawUtils::drawText(textPos, &txt, MC_Color(255, 255, 255), size, alpha);
			if (this->showCoordinates) {
				textPos.y += textHeight + 1.f * size;
				
				DrawUtils::drawText(textPos, &coordText, MC_Color(255, 255, 255), size * 0.75f, alpha);
			}
		}
		DrawUtils::flush();
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
				int dim = 0;
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
				if (val.contains("dim")) {
					auto dimVal = val.at("dim");
					if (!dimVal.is_null() && dimVal.is_number_integer()) {
						try {
							dim = dimVal.get<int>();
						} catch (std::exception e) {
						}
					}
				}
				waypoints->emplace(it.key().c_str(), WaypointInstance(_pos, dim));
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
		auto wp = it->second;
		subObj["pos"]["x"] = (float)wp.pos.x;
		subObj["pos"]["y"] = (float)wp.pos.y;
		subObj["pos"]["z"] = (float)wp.pos.z;
		subObj["dim"] = wp.dimension;
		myList.emplace(it->first.c_str(), subObj);
	}

	if (waypoints->size() > 0) {
		obj.emplace("list", myList);
	}

	conf->emplace(modName.c_str(), obj);
}
