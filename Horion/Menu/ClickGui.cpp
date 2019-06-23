#include "ClickGui.h"

// Render


bool isLeftClickDown = false;
bool shouldToggle = false;
bool isRightClickDown = false;

bool isDragging = false;

void ClickGui::getModuleListByCategory(Category category, std::vector<IModule*>* modList) {
	std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

	for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
		if ((*it)->getCategory() == category)
			modList->push_back(*it);
	}
}

void ClickGui::renderCategory(Category category)
{
	static constexpr float textPadding = 1.0f;
	static constexpr float textSize = 1.0f;
	static constexpr float textHeight = textSize * 10.0f;

	const float yOffset = 4;
	float currentYOffset = yOffset;
	const float xOffset = 150;
	
	const char* categoryName;
	// Get Category Name
	{
		switch (category) {
		case COMBAT:
			categoryName = "Combat";
			break;
		case VISUAL:
			categoryName = "Visual";
			break;
		case MOVEMENT:
			categoryName = "Movement";
			break;
		case BUILD:
			categoryName = "Build";
			break;
		case EXPLOITS:
			categoryName = "Exploits";
			break;
		}
	}
	
	// Get All Modules in our category
	std::vector<IModule*> moduleList;
	getModuleListByCategory(category, &moduleList);

	float maxLength = 1;
	// Get max width of all text
	{
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::string label = (*it)->getModuleName();
			maxLength = max(maxLength, DrawUtils::getTextWidth(&label, textSize, SMOOTH));
		}
	}

	const float xEnd = xOffset + maxLength + 10.5f;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	// Convert mousePos to visual Pos
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		mousePos.div(windowSizeReal);
		mousePos.mul(windowSize);
	}

	// Draw Category Name
	{
		vec2_t textPos = vec2_t(
			xOffset + textPadding,
			currentYOffset + textPadding
		);
		vec4_t rectPos = vec4_t(
			xOffset,
			currentYOffset,
			xOffset + maxLength + 10.5f,
			currentYOffset + textHeight
		);

		std::string textStr = categoryName;
		DrawUtils::drawText(textPos, &textStr, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), 1.0f);
		// Draw Dash
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, false);
		currentYOffset += textHeight + (textPadding * 2);
	}
	
	// Loop through mods to display Labels
	for (std::vector<IModule*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		std::string textStr = (*it)->getModuleName();

		vec2_t textPos = vec2_t(
			xOffset + textPadding,
			currentYOffset + textPadding
		);
		vec4_t rectPos = vec4_t(
			xOffset,
			currentYOffset,
			xEnd,
			currentYOffset + textHeight
		);
		
		if (rectPos.contains(&mousePos)) { // Is the Mouse hovering above us?
			DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f), 1.0f);
			if (shouldToggle) { // Are we being clicked?
				(*it)->toggle();
				shouldToggle = false;
			}
		}
		else {
			DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.f, 0.f, 1.f), 1.0f);
		}
		DrawUtils::drawText(textPos, &textStr, (*it)->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, true);

		currentYOffset += textHeight + (textPadding * 2);
	}
	
	DrawUtils::flush();
	moduleList.clear();
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;

	// Fill Background
	{
		DrawUtils::fillRectangle(vec4_t(
			0,
			0,
			g_Data.getClientInstance()->getGuiData()->widthGame,
			g_Data.getClientInstance()->getGuiData()->heightGame
		), MC_Color(0.8f, 0.8f, 0.8f, 0.1f), 0.1f);
	}

	// Render all categorys
	renderCategory(COMBAT);
	renderCategory(VISUAL);
	renderCategory(MOVEMENT);
	renderCategory(BUILD);
	renderCategory(EXPLOITS);

	isLeftClickDown = false;
	isRightClickDown = false;
	shouldToggle = false;
}

void ClickGui::init() {
}

void ClickGui::onMouseClickUpdate(int key, bool isDown)
{
	switch (key) {
	case 0: // Left Click
		isLeftClickDown = isDown;
		if (isDown)
			shouldToggle = true;
		break;
	}
	
}
