#include "ClickGui.h"

bool isLeftClickDown = false; 
bool isRightClickDown = false;
bool shouldToggleLeftClick = false; // If true, toggle the focused module
bool shouldToggleRightClick = false;
bool resetStartPos = true;

std::map<unsigned int, std::shared_ptr<ClickWindow>> windowMap;

bool isDragging = false;
unsigned int draggedWindow = -1;
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
	int seed = 0;
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

unsigned int ClickGui::getWindowHash(const char* name) {
	return getCrcHash(name);
}

std::shared_ptr<ClickWindow> ClickGui::getWindow(const char * name)
{
	unsigned int id = getWindowHash(name);

	auto search = windowMap.find(id);
	if (search != windowMap.end()) { // Window exists already
		return search->second;
	}
	else { // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickWindow> newWindow = std::make_shared<ClickWindow>();

		windowMap.insert(std::make_pair(id, newWindow));
		return newWindow;
	}
}

void ClickGui::renderCategory(Category category)
{
	static constexpr float textPadding = 1.0f;
	static constexpr float textSize = 1.0f;
	static constexpr float textHeight = textSize * 10.0f;
	static constexpr float paddingRight = 13.5f;
	static constexpr float crossSize = textHeight / 2.f;
	static constexpr float crossWidth = 0.3f;
	
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

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos) {
		ourWindow->pos.y = 4;
		switch (category) {
		case COMBAT:
			ourWindow->pos.x = 150;
			break;
		case VISUAL:
			ourWindow->pos.x = 250;
			break;
		case MOVEMENT:
			ourWindow->pos.x = 350;
			break;
		case BUILD:
			ourWindow->pos.x = 450;
			break;
		case EXPLOITS:
			ourWindow->pos.x = 550;
			break;
		}
	}

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

	const float xEnd = xOffset + maxLength + paddingRight;

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
			xOffset + maxLength + paddingRight,
			currentYOffset + textHeight + (textPadding * 2)
		);

		// Extend Logic
		{
			if (rectPos.contains(&mousePos) && shouldToggleRightClick && !isDragging) {
				shouldToggleRightClick = false;
				ourWindow->isExtended = !ourWindow->isExtended;
			}
		}

		// Dragging Logic
		{
			if (isDragging && getWindowHash(categoryName) == draggedWindow) { // WE are being dragged
				if (isLeftClickDown) { // Still dragging
					vec2_t diff = vec2_t(mousePos).sub(dragStart);
					ourWindow->pos.add(diff);
					dragStart = mousePos;
				}
				else { // Stopped dragging
					isDragging = false;
				}
			}
			else if (rectPos.contains(&mousePos) && shouldToggleLeftClick) {
				isDragging = true;
				draggedWindow = getWindowHash(categoryName);
				shouldToggleLeftClick = false;
				dragStart = mousePos;
			}
		}
		
		// Draw component
		{
			// Draw Text
			std::string textStr = categoryName;
			DrawUtils::drawText(textPos, &textStr, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
			DrawUtils::fillRectangle(rectPos, MC_Color(0.118f, 0.827f, 0.764f, 1.f), 0.95f);
			// Draw Dash
			GuiUtils::drawCrossLine(vec2_t(xOffset + maxLength + paddingRight - (crossSize / 2) - 1.f, currentYOffset + textPadding + (textHeight / 2)), MC_Color(1.0f, 0.2f, 0, 1.0f), crossWidth, crossSize, !ourWindow->isExtended);
			currentYOffset += textHeight + (textPadding * 2);
		}
		
	}

	// Loop through mods to display Labels
	if (ourWindow->isExtended) {
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
				currentYOffset + textHeight + (textPadding * 2)
			);

			if (rectPos.contains(&mousePos)) { // Is the Mouse hovering above us?
				DrawUtils::fillRectangle(rectPos, MC_Color(0.4f, 0.9f, 0.4f, 1.f), 0.8f);
				if (shouldToggleLeftClick) { // Are we being clicked?
					(*it)->toggle();
					shouldToggleLeftClick = false;
				}
			}
			else {
				DrawUtils::fillRectangle(rectPos, MC_Color(0.2f, 0.2f, 0.2f, 1.f), 0.7f);
			}
			if (rectPos.contains(&mousePos) && shouldToggleRightClick) {
				shouldToggleRightClick = false;
				(*it)->setExtended(!(*it)->isExtended());
			}
			DrawUtils::drawText(textPos, &textStr, (*it)->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
			GuiUtils::drawCrossLine(vec2_t(xOffset + maxLength + paddingRight - (crossSize / 2) - 1.f, currentYOffset + textPadding + (textHeight / 2)), MC_Color(1.0f, 0.2f, 0, 1.0f), crossWidth, crossSize,!(*it)->isExtended());

			currentYOffset += textHeight + (textPadding * 2);
		}
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

	shouldToggleLeftClick = false; 
	shouldToggleRightClick = false;
	resetStartPos = false;
}

void ClickGui::init() { }

void ClickGui::onMouseClickUpdate(int key, bool isDown)
{
	switch (key) {
	case 0: // Left Click
		isLeftClickDown = isDown;
		if (isDown)
			shouldToggleLeftClick = true;
		break;
	case 1: // Right Click
		isRightClickDown = isDown;
		if (isDown)
			shouldToggleRightClick = true;
		break;
	}
	
}
