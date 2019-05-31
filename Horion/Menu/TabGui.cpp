#include "TabGui.h"


// State
int level;
int selected[4];

// Render
float yOffset;
float xOffset;
int renderedLevel;

std::vector<const char*> labelList;

void TabGui::renderLabel(const char * text)
{
	size_t strlength = strlen(text) + 1;
	char* alloc = new char[strlength];
	strcpy_s(alloc, strlength, text);
	labelList.push_back(alloc);
}

void TabGui::renderLevel()
{
	// Parameters
	static constexpr float textSize = 1.f;
	static constexpr float textHeight = 10.f * textSize;

	// First loop: Get the maximum text length 
	float maxLength = 1;
	int labelListLength = 0;
	for (std::vector<const char*>::iterator it = labelList.begin(); it != labelList.end(); ++it) {
		labelListLength++;
		const char* label = *it;
		std::string text = label;
		maxLength = max(maxLength, DrawUtils::getTextLength(&text, textSize, SMOOTH));
	}

	if (selected[renderedLevel] < 0)
		selected[renderedLevel] = labelListLength + selected[renderedLevel];
	if (selected[renderedLevel] >= labelListLength)
		selected[renderedLevel] -= labelListLength;

	// Second loop: Render everything
	int i = 0;
	float selectedYOffset = yOffset;
	for (std::vector<const char*>::iterator it = labelList.begin(); it != labelList.end(); ++it) {
		const char* label = *it;
		std::string text = label;
		vec4_t rectPos = vec4_t(
			xOffset - 0.5f,  // Off screen / Left border not visible
			yOffset,
			xOffset + maxLength + 4.f,
			yOffset + textHeight);
		
		if (selected[renderedLevel] == i) {
			// If selected, show in green
			if(renderedLevel != level) // Are we actually in the menu we are drawing right now?
				DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.3f, 0.3f, 1.0f), 0.15f); // No we are not
			else 
				DrawUtils::fillRectangle(rectPos, MC_Color(0.3f, 0.8f, 0.3f, 1.0f), 0.5f); // Yes we are!
			selectedYOffset = yOffset;
		}
		else
			DrawUtils::fillRectangle(rectPos, MC_Color(0.8f, 0.8f, 0.8f, 1.0f), 0.1f);
		DrawUtils::drawRectangle(rectPos, MC_Color(0.0f, 0.0f, 0.0f, 1.0f), 1.0f); // Border around Text

		DrawUtils::drawText(vec2_t(xOffset + 1.f, yOffset), &text, /* White Color*/ nullptr, textSize, SMOOTH);

		yOffset += textHeight;
		i++;
	}
	// Cleanup
	DrawUtils::flush();
	for (std::vector<const char*>::iterator it = labelList.begin(); it != labelList.end(); ++it) {
		const char* label = *it;
		delete[] label;
	}
	labelList.clear();
	xOffset += maxLength + 4.5f;
	yOffset = selectedYOffset;
	renderedLevel++;
}

void TabGui::render()
{
	if (!moduleMgr->isInitialized())
		return;
	else
		return;
	renderedLevel = 0;
	yOffset = 15;
	xOffset = 3;

	// Render all categorys
	renderLabel("Combat");
	renderLabel("Visual");
	renderLabel("Movement");
	renderLabel("Build");
	renderLabel("Exploits");
	renderLevel();
	
	std::vector<IModule*>* modules = moduleMgr->getModuleList();
	for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
		IModule* mod = *it;
		if (selected[0] == mod->getCategory()) {
			//std::string yikes = mod->getModuleName();
			for(int i = 0; i < 10; i++)
				renderLabel("Not too bad");
		}
	}
	renderLevel();
	
}

void TabGui::init() {
	level = 0;
	xOffset = 0;
	yOffset = 0;
	renderedLevel = 0;
}

void TabGui::onKeyUpdate(int key, bool isDown)
{
	if (!isDown)
		return;

	switch (key) {
	case VK_LEFT: // Leave menus
		if (level > 0) {
			level--;
		}
		return;
	case VK_RIGHT:
		if (level < 3) {
			level++;
			selected[level] = 0;
		}
			
		return;
	case VK_UP:
		selected[level]--;
		break;
	case VK_DOWN:
		selected[level]++;
		break;
	};

	if(level < 3)
		selected[level + 1] = 0;
}
