#include "ClickGui.h"

// Render
static float yOffset;
static float xOffset;

bool isdown;

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

	const char* categoryName;
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
	
	std::vector<IModule*> moduleList;
	getModuleListByCategory(category, &moduleList);

	float maxLength = 1;

	for (auto it = moduleList.begin(); it != moduleList.end(); ++it) {
		std::string label = (*it)->getModuleName();
		maxLength = max(maxLength, DrawUtils::getTextWidth(&label, textSize, SMOOTH));
	}

	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	mousePos.div(windowSizeReal);
	mousePos.mul(windowSize);

	DrawUtils::fillRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame,
		g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.025f);

	std::string textStr = categoryName;
	float textWidth = DrawUtils::getTextWidth(&textStr);

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

	DrawUtils::drawText(textPos, &textStr, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
	DrawUtils::fillRectangle(rectPos, MC_Color(0.f, 0.1f, 0.1f, 0.1f), 1.0f);
	GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, false);
	yOffset += textHeight + (textPadding * 2);

	// Loop through mods to display Labels
	for (std::vector<IModule*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {

		textStr = (*it)->getModuleName();
		textWidth = DrawUtils::getTextWidth(&textStr);

		textPos = vec2_t(
			xOffset + textPadding,
			yOffset + textPadding
		);
		rectPos = vec4_t(
			xOffset,
			yOffset,
			xOffset + maxLength + 10.5f,
			yOffset + textHeight
		);
		DrawUtils::drawText(textPos, &textStr, (*it)->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
		if (!GameData::canUseMoveKeys() && rectPos.contains(&mousePos)) {

			DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 0.1f),1.0f);
			if (isdown) {
				(*it)->toggle();
				isdown = false;
			}
		}
		GuiUtils::drawCrossLine(vec4_t(rectPos.z - 8.0f, rectPos.y + 1.0f, rectPos.z - 1.0f, rectPos.w - 1.0f), MC_Color(1.0f, 0.2f, 0, 1.0f), 0.5f, true);

		yOffset += textHeight + (textPadding * 2);
	}
	DrawUtils::fillRectangle(vec4_t(xOffset, 4, rectPos.z,yOffset), MC_Color(0.f, 0.1f, 0.1f, 0.1f), 0.4f);
	DrawUtils::flush();
	moduleList.clear();
	yOffset = 4;
	xOffset += 100;
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;
	yOffset = 4;
	xOffset = 150;
	// Render all categorys
	renderCategory(COMBAT);
	renderCategory(VISUAL);
	renderCategory(MOVEMENT);
	renderCategory(BUILD);
	renderCategory(EXPLOITS);
}

void ClickGui::init() {
	yOffset = 4;
	xOffset = 100;
}

void ClickGui::onMouseClickUpdate(bool isDown)
{
	isdown = isDown;
}
