#include "TabGui.h"

// State
int level;
int selected[4];
bool toggleCurrentSelection = false;

// Render
static float yOffset;
static float xOffset;
int renderedLevel;

struct LabelContainer {
	const char* text = 0;
	bool enabled = false;
	IModule* mod = 0;
};

std::vector<LabelContainer> labelList;

void TabGui::renderLabel(const char* text, IModule* mod) {
	//size_t strlength = strlen(text) + 1;
	//char* alloc = new char[strlength];
	//strcpy_s(alloc, strlength, text);
	LabelContainer yikes;
	yikes.text = text;
	if (mod != 0) {
		yikes.enabled = mod->isEnabled();
		yikes.mod = mod;
	}

	labelList.push_back(yikes);
}

void TabGui::renderLevel() {
	// Parameters
	static constexpr float textSize = 1.f;
	static const float textHeight = 10.f * textSize;
	static constexpr float alphaVal = 1.0f;

	// First loop: Get the maximum text length
	float maxLength = 1;
	int labelListLength = 0;
	for (auto it = labelList.begin(); it != labelList.end(); ++it) {
		labelListLength++;
		std::string label = it->text;
		maxLength = max(maxLength, DrawUtils::getTextWidth(&label, textSize));
	}

	if (selected[renderedLevel] < 0)
		selected[renderedLevel] = labelListLength + selected[renderedLevel];
	if (selected[renderedLevel] >= labelListLength)
		selected[renderedLevel] -= labelListLength;

	// Second loop: Render everything
	int i = 0;
	float selectedYOffset = yOffset;
	for (auto it = labelList.begin(); it != labelList.end(); ++it) {
		auto label = *it;
		vec4_t rectPos = vec4_t(
			xOffset - 0.5f,  // Off screen / Left border not visible
			yOffset,
			xOffset + maxLength + 4.5f,
			yOffset + textHeight);

		MC_Color* color = new MC_Color(200, 200, 200, 1);

		if (selected[renderedLevel] == i && level >= renderedLevel) {  // We are selected
			if (renderedLevel == level) {                              // Are we actually in the menu we are drawing right now?
				// We are selected in the current menu
				DrawUtils::fillRectangle(rectPos, MC_Color(28, 107, 201, 1), alphaVal);
				static bool lastVal = toggleCurrentSelection;

				if (toggleCurrentSelection) {
					if (label.mod->isFlashMode()) {
						label.mod->setEnabled(true);
					} else {
						toggleCurrentSelection = false;
						label.mod->toggle();
					}
				} else if (toggleCurrentSelection != lastVal && label.mod->isFlashMode())
					label.mod->setEnabled(false);
				lastVal = toggleCurrentSelection;
			} else {
				DrawUtils::fillRectangle(rectPos, MC_Color(28, 107, 201, 1), alphaVal);
			}
			selectedYOffset = yOffset;
		} else {  // We are not selected
			DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48, 1), 1.f);
		}

		//DrawUtils::drawRectangle(rectPos, MC_Color(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.3f); // Border around Text
		std::string tempLabel(label.text);
		DrawUtils::drawText(vec2_t(xOffset + 1.5f, yOffset /*+ 0.5f*/), &tempLabel, label.enabled ? nullptr : color, textSize);

		yOffset += textHeight;
		i++;
	}
	// Cleanup
	DrawUtils::flush();
	labelList.clear();
	xOffset += maxLength + 4.5f;
	yOffset = selectedYOffset;
	renderedLevel++;
}

void TabGui::render() {
	if (!moduleMgr->isInitialized())
		return;
	renderedLevel = 0;
	yOffset = 4;
	xOffset = 3;

	// Render all categorys
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Player");
	renderLabel("Build");
	renderLabel("Exploits");
	renderLevel();

	// Render all modules
	if (level >= 0) {
		std::vector<IModule*>* modules = moduleMgr->getModuleList();
		for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
			IModule* mod = *it;
			if (selected[0] == static_cast<int>(mod->getCategory())) {
				auto name = mod->getModuleName();
				renderLabel(name, mod);
			}
		}
		renderLevel();
	}
}

void TabGui::init() {
	level = 0;
	xOffset = 0;
	yOffset = 0;
	renderedLevel = 0;
}

void TabGui::onKeyUpdate(int key, bool isDown) {
	if (!isDown) {
		if (key == VK_RIGHT)
			toggleCurrentSelection = false;
		return;
	}

	switch (key) {
	case VK_LEFT:  // Leave menus
		if (level > -1) {
			level--;
		}
		return;
	case VK_RIGHT:
		if (level < 1) {
			level++;
			selected[level] = 0;
		} else
			toggleCurrentSelection = true;
		return;
	case VK_UP:
		if (level >= 0)
			selected[level]--;
		else
			level = 0;
		break;
	case VK_DOWN:
		if (level >= 0)
			selected[level]++;
		else
			level = 0;
		break;
	};

	if (level < 3)
		selected[level + 1] = 0;
}
