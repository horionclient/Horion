#include "ClickGui.h"


std::map<int, std::shared_ptr<ClickWindow>> windowMap;

bool isLeftClickDown = false;
bool shouldToggle = false;
bool isRightClickDown = false;

bool isDragging = false;
vec2_t dragStart = vec2_t();

void ClickGui::getModuleListByCategory(Category category, std::vector<IModule*>* modList) {
	std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

	for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
		if ((*it)->getCategory() == category)
			modList->push_back(*it);
	}
}

// Stolen from IMGUI
unsigned int ClickGui::getCrcHash(const char * str)
{
	static unsigned int crc32_lut[256] = { 0 };
	static int seed = 0;
	if (!crc32_lut[1])
	{
		const unsigned int polynomial = 0xEDB88320;
		for (unsigned int i = 0; i < 256; i++)
		{
			unsigned int crc = i;
			for (unsigned int j = 0; j < 8; j++)
				crc = (crc >> 1) ^ (unsigned int(-int(crc & 1)) & polynomial);
			crc32_lut[i] = crc;
		}
	}

	seed = ~seed;
	unsigned int crc = seed;
	const unsigned char* current = (const unsigned char*)str;
	{
		// Zero-terminated string
		while (unsigned char c = *current++)
		{
			// We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
			// Because this syntax is rarely used we are optimizing for the common case.
			// - If we reach ### in the string we discard the hash so far and reset to the seed.
			// - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
			if (c == '#' && current[0] == '#' && current[1] == '#')
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
		}
	}
	return ~crc;
}

std::shared_ptr<ClickWindow> ClickGui::getWindow(const char * name)
{
	int id = getCrcHash(name);
	auto search = windowMap.find(id);
	if (search != windowMap.end()) { // Window exists already
		return search->second;
	}
	else { // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickWindow> newWindow = std::make_shared<ClickWindow>();
		windowMap[id] = newWindow;
		return newWindow;
	}
}

void ClickGui::renderCategory(Category category)
{
	static constexpr float textPadding = 1.0f;
	static constexpr float textSize = 1.0f;
	static constexpr float textHeight = textSize * 10.0f;
	
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

	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);

	const float xOffset = ourWindow->pos.x;
	const float yOffset = ourWindow->pos.y;
	float currentYOffset = yOffset;
	
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

		if (isDragging) {
			if (isLeftClickDown) { // Still dragging
				vec2_t diff = vec2_t(mousePos).sub(dragStart);
				diff.mul(2);
				ourWindow->pos.add(diff);
				dragStart = mousePos;
			}
			else { // Stopped dragging
				logF("stopped dragging");
				isDragging = false;
			}
		} else if (rectPos.contains(&mousePos) && shouldToggle) {
			logF("started dragging");
			isDragging = true;
			shouldToggle = false;
			dragStart = mousePos;
		}

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
