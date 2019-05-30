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
	labelList.push_back(text);
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

		DrawUtils::drawText(vec2_t(xOffset + 0.5f, yOffset), &text, /* White Color*/ nullptr, textSize, SMOOTH);

		yOffset += textHeight;
		i++;
	}
	// Cleanup
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
	yOffset = 15;
	xOffset = 3;

	// Render all categorys
	renderLabel("Yikes");
	renderLabel("Yeeeet");
	renderLabel("Yeehawt");
	renderLabel("Yikes");
	renderLabel("Yeeeet");
	renderLabel("Yeehawt");
	renderLevel();

	bool couldFindLevel = true;
	switch (selected[0]) {
	case 0:
		renderLabel("Yikes:");
		renderLabel("Thanks for the Yikes!");
		break;
	case 1:
		renderLabel("Yeeeet:");
		renderLabel("Yeeeet!");
		break;
	case 2:
		renderLabel("Yeehawt:");
		renderLabel("Hell yea brother!");
		break;
	default:
		couldFindLevel = false;
		if (level > 0)
			level = 0;
	}
	if (couldFindLevel)
		renderLevel();
	/*std::vector<IModule*>* modules = moduleMgr->getModuleList();
	for (std::vector<IModule*>::iterator it = modules->begin(); it != modules->end(); ++it) {
		IModule* 
	}*/
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
		if (level > 0)
			level--;
		break;
	case VK_RIGHT:
		if (level < 3) {
			level++;
			selected[level] = 0;
		}
			
		break;
	case VK_UP:
		selected[level]--;
		break;
	case VK_DOWN:
		selected[level]++;
		break;
	};
}
