#include "ClickGui.h"

// Render


bool isLeftClickDown = false;
bool isRightClickDown = false;

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

	float yOffset = 4;
	float xOffset = 150;

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

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	// Convert mousePos to visual Pos
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		mousePos.div(windowSizeReal);
		mousePos.mul(windowSize);
	}
	
	// Fill Background
	DrawUtils::fillRectangle(vec4_t(
		0, 
		0, 
		g_Data.getClientInstance()->getGuiData()->widthGame,
		g_Data.getClientInstance()->getGuiData()->heightGame
	), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.025f);

	// Render Category Name
	{
		vec2_t textPos = vec2_t(
			xOffset + textPadding,
			yOffset + textPadding
		);
		vec4_t rectPos = vec4_t(
			xOffset,
			yOffset,
			xOffset + maxLength + 10.5f,
			yOffset + textHeight
		);

		std::string textStr = categoryName;
		DrawUtils::drawText(textPos, &textStr, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), 1.0f);
		// Draw Dash
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, false);
		yOffset += textHeight + (textPadding * 2);
	}
	
	// Loop through mods to display Labels
	for (std::vector<IModule*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
		std::string textStr = (*it)->getModuleName();

		vec2_t textPos = vec2_t(
			xOffset + textPadding,
			yOffset + textPadding
		);
		vec4_t rectPos = vec4_t(
			xOffset,
			yOffset,
			xOffset + maxLength + 10.5f,
			yOffset + textHeight
		);
		DrawUtils::drawText(textPos, &textStr, (*it)->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos)) {

			DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f),1.0f);
			if (isLeftClickDown) {
				(*it)->toggle();
				isLeftClickDown = false;
			}
		}
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, true);

		yOffset += textHeight + (textPadding * 2);
	}
	DrawUtils::fillRectangle(vec4_t(
		xOffset, 
		4, 
		xOffset + maxLength + 10.5f,
		yOffset), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.4f);
	DrawUtils::flush();
	moduleList.clear();
	
	xOffset += 100;
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;
	// Render all categorys
	renderCategory(COMBAT);
	renderCategory(VISUAL);
	renderCategory(MOVEMENT);
	renderCategory(BUILD);
	renderCategory(EXPLOITS);

	isLeftClickDown = false;
	isRightClickDown = false;
}

void ClickGui::init() {
}

void ClickGui::onMouseClickUpdate(int key, bool isDown)
{
	switch (key) {
	case 0: // Left Click
		isLeftClickDown = isDown;
		break;
	}
	
}
