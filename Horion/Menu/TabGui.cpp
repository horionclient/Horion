#include "TabGui.h"


// State
int level;
int selected[4];
bool toggleCurrentSelection = false;

// Render
float yOffset;
float xOffset;
int renderedLevel;

struct LabelContainer {
	const char* text;
	bool enabled = false;
	IModule* mod = 0;
};

std::vector<LabelContainer> labelList;

void TabGui::renderLabel(const char * text, IModule* mod)
{
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

void TabGui::renderLevel()
{
	// Parameters
	static constexpr float textSize = 1.f;
	static constexpr float textHeight = 11.f * textSize;
	static constexpr float alphaVal = 0.8f;

	// First loop: Get the maximum text length 
	float maxLength = 1;
	int labelListLength = 0;
	for (auto it = labelList.begin(); it != labelList.end(); ++it) {
		labelListLength++;
		std::string label = it->text;
		maxLength = max(maxLength, DrawUtils::getTextWidth(&label, textSize, SMOOTH));
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
			xOffset + maxLength + 4.f,
			yOffset + textHeight);
		
		if (selected[renderedLevel] == i && level >= renderedLevel) { // We are selected
			if (renderedLevel == level) { // Are we actually in the menu we are drawing right now?
				// We are selected in the current menu

				if (label.mod == 0) {
					// Category
					DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.3f, 0.3f, 1.0f), alphaVal);
				}
				else {
					// Modules
					if(label.enabled)
						DrawUtils::fillRectangle(rectPos, MC_Color(0.45f, 0.575f, 0.45f, 1.0f), alphaVal);
					else
						DrawUtils::fillRectangle(rectPos, MC_Color(0.45f, 0.45f, 0.45f, 1.0f), alphaVal);

					static bool lastVal = toggleCurrentSelection;

					if (toggleCurrentSelection) {
						if (label.mod->isFlashMode()) {
							label.mod->setEnabled(true);
						}
						else {
							toggleCurrentSelection = false;
							label.mod->toggle();
						}
					}
					else if (toggleCurrentSelection != lastVal && label.mod->isFlashMode())
						label.mod->setEnabled(false);
					lastVal = toggleCurrentSelection;
				}
			}
			else {
				// We are selected but we are not in the current menu
				DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.3f, 0.3f, 1.0f), alphaVal);
			}
			selectedYOffset = yOffset;
		}
		else { // We are not selected
			if (label.enabled && renderedLevel > 0)
				DrawUtils::fillRectangle(rectPos, MC_Color(0.5f, 0.7f, 0.5f, 1.0f), alphaVal);
			else if(renderedLevel > 0)
				DrawUtils::fillRectangle(rectPos, MC_Color(0.7f, 0.45f, 0.45f, 1.0f), alphaVal);
			else
				DrawUtils::fillRectangle(rectPos, MC_Color(0.2f, 0.2f, 0.2f, 1.0f), alphaVal);
		}
			
		//DrawUtils::drawRectangle(rectPos, MC_Color(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.5f); // Border around Text

		DrawUtils::drawText(vec2_t(xOffset + 1.f, yOffset), &std::string(label.text), /* White Color*/ nullptr, textSize, SMOOTH);

		yOffset += textHeight;
		i++;
	}
	// Cleanup
	DrawUtils::flush();
	/*for (auto it = labelList.begin(); it != labelList.end(); ++it) {
		auto label = *it;
		delete[] label;
	}*/
	labelList.clear();
	xOffset += maxLength + 4.5f;
	yOffset = selectedYOffset;
	renderedLevel++;
}

void TabGui::render()
{
	if (!moduleMgr->isInitialized())
		return;
	renderedLevel = 0;
	yOffset = 13;
	xOffset = 3;

	// Render all categorys
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Build");
	renderLabel("Exploits");
	renderLevel();
	
	// Render all modules
	if (level >= 0) {
		std::vector<IModule*>* modules = moduleMgr->getModuleList();
		for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
			IModule* mod = *it;
			if (selected[0] == mod->getCategory()) {
				auto yikes = mod->getModuleName();
				renderLabel(yikes, mod);
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

void TabGui::onKeyUpdate(int key, bool isDown)
{
	if (!isDown) {
		if(key == VK_RIGHT)
			toggleCurrentSelection = false;
		return;
	}
		

	switch (key) {
	case VK_LEFT: // Leave menus
		if (level > -1) {
			level--;
		}
		return;
	case VK_RIGHT:
		if (level < 1) {
			level++;
			selected[level] = 0;
		}
		else
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

	if(level < 3)
		selected[level + 1] = 0;
}
