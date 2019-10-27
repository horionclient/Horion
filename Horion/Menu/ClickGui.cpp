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

unsigned int focusedElement = -1;
bool isFocused = false;

static constexpr float textPadding = 1.0f;
static constexpr float textSize = 1.0f;
static constexpr float textHeight = textSize * 10.0f;
static constexpr float categoryMargin = 0.5f;
static constexpr float paddingRight = 13.5f;
static constexpr float crossSize = textHeight / 2.f;
static constexpr float crossWidth = 0.3f;
static const MC_Color selectedModuleColor = MC_Color(0.5f, 0.5f, 0.5f, 1.f);
static const MC_Color moduleColor = MC_Color(0.2f, 0.2f, 0.2f, 1.f);

float currentYOffset = 0;
float currentXOffset = 0;

int timesRendered = 0;

void ClickGui::getModuleListByCategory(Category category, std::vector<IModule*>* modList) {
	std::vector<IModule*>* moduleList = moduleMgr->getModuleList();

	for (std::vector<IModule*>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
		if ((*it)->getCategory() == category)
			modList->push_back(*it);
	}
}

// Stolen from IMGUI
unsigned int ClickGui::getCrcHash(const char * str, int seed)
{
	static unsigned int crc32_lut[256] = { 0 };
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
	unsigned int id = getCrcHash(name);

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

std::shared_ptr<ClickModule> ClickGui::getClickModule(std::shared_ptr<ClickWindow> window, const char * name)
{
	unsigned int id = getCrcHash(name);

	auto search = window->moduleMap.find(id);
	if (search != window->moduleMap.end()) { // Window exists already
		return search->second;
	}
	else { // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickModule> newModule = std::make_shared<ClickModule>();

		window->moduleMap.insert(std::make_pair(id, newModule));
		return newModule;
	}
}

void ClickGui::renderLabel(const char * text)
{

}

void ClickGui::renderTooltip(std::string* text, vec2_t mousepos) {		

	float textWidth = DrawUtils::getTextWidth(text, 1.25f);
	vec2_t textPos = vec2_t(
		mousepos.x + textPadding + 12.f,
		mousepos.y + textPadding
	);
	vec4_t rectPos = vec4_t(
		mousepos.x + 12.f,
		mousepos.y,
		mousepos.x + paddingRight + textWidth,
		mousepos.y + textHeight + (textPadding * 2)
	);
	DrawUtils::fillRectangle(rectPos, MC_Color(0.2f, 0.2f, 0.2f, 1.0f), 1.0f);
	DrawUtils::drawText(textPos, text, new MC_Color(1.f, 1.f, 1.f, 1.f),1.05f);
}

void ClickGui::renderCategory(Category category)
{
	const char* categoryName;

	// Get Category Name
	{
		switch (category) {
		case Category::COMBAT:
			categoryName = "Combat";
			break;
		case Category::VISUAL:
			categoryName = "Visual";
			break;
		case Category::MOVEMENT:
			categoryName = "Movement";
			break;
		case Category::PLAYER:
			categoryName = "Player";
			break;
		case Category::BUILD:
			categoryName = "Build";
			break;
		case Category::EXPLOITS:
			categoryName = "Exploits";
			break;
		}
	}
	
	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos) {
		ourWindow->pos.y = 4;
		switch (category) {
		case Category::COMBAT:
			ourWindow->pos.x = 100;
			break;
		case Category::VISUAL:
			ourWindow->pos.x = 200;
			break;
		case Category::MOVEMENT:
			ourWindow->pos.x = 300;
			break;
		case Category::PLAYER:
			ourWindow->pos.x = 400;
			break;
		case Category::BUILD:
			ourWindow->pos.x = 500;
			break;
		case Category::EXPLOITS:
			ourWindow->pos.x = 600;
			break;
		}
	}

	const float xOffset = ourWindow->pos.x;
	const float yOffset = ourWindow->pos.y;
	vec2_t* windowSize = &ourWindow->size;
	currentXOffset = xOffset;
	currentYOffset = yOffset;

	// Get All Modules in our category
	std::vector<IModule*> moduleList;
	getModuleListByCategory(category, &moduleList);
	
	// Get max width of all text
	{
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::string label = (*it)->getModuleName();
			windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&label, textSize, SMOOTH));
		}
	}

	const float xEnd = currentXOffset + windowSize->x + paddingRight;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();
	// Convert mousePos to visual Pos and anti idiot
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		mousePos.div(windowSizeReal);
		mousePos.mul(windowSize);

		if (ourWindow->pos.x + ourWindow->size.x > windowSize.x) {
			ourWindow->pos.x = windowSize.x - ourWindow->size.x;
		}

		if (ourWindow->pos.y + ourWindow->size.y > windowSize.y) {
			ourWindow->pos.y = windowSize.y - ourWindow->size.y;
		}

		ourWindow->pos.x = max(0, ourWindow->pos.x);
		ourWindow->pos.y = max(0, ourWindow->pos.y);
	}


	float categoryHeaderYOffset = currentYOffset;

	if (ourWindow->isInAnimation) {
		if (ourWindow->isExtended) {
			ourWindow->animation -= 0.05f;
			if (ourWindow->animation <= 0)
				ourWindow->isInAnimation = false;
		}
		else {
			ourWindow->animation += 0.05f;
			if (ourWindow->animation >= 1)
				ourWindow->isInAnimation = false;
		}
	}

	currentYOffset += textHeight + (textPadding * 2);
	// Loop through Modules to display em	
	if (ourWindow->isExtended || ourWindow->isInAnimation) {
		if (ourWindow->isInAnimation) {
			currentYOffset -= pow(ourWindow->animation * 10, 3);
			shouldToggleLeftClick = false; // Disable Toggles during animations because they can be buggy af
		}
			
		for (std::vector<IModule*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			IModule* mod = *it;
			std::string textStr = mod->getModuleName();

			vec2_t textPos = vec2_t(
				currentXOffset + textPadding,
				currentYOffset + textPadding
			);
			vec4_t rectPos = vec4_t(
				currentXOffset,
				currentYOffset,
				xEnd,
				currentYOffset + textHeight + (textPadding * 2)
			);

			bool allowRender = currentYOffset >= categoryHeaderYOffset;

			// Background
			if(allowRender)
			{
				if (rectPos.contains(&mousePos)) { // Is the Mouse hovering above us?
					DrawUtils::fillRectangle(rectPos, selectedModuleColor, 0.8f);
					std::string tooltip = mod->getTooltip();
					ClickGuiMod* clickgui = moduleMgr->getModule<ClickGuiMod>();
					if(clickgui->showTooltips) renderTooltip(&tooltip, mousePos);
					if (shouldToggleLeftClick) { // Are we being clicked?
						mod->toggle();
						shouldToggleLeftClick = false;
					}
				}
				else {
					DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f);
				}
			}
			
			// Text
			if (allowRender)
				DrawUtils::drawText(textPos, &textStr, mod->isEnabled() ? new MC_Color(0, 1.0f, 0, 1.0f) : new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);

			// Settings
			{
				std::vector<SettingEntry*>* settings = mod->getSettings();
				if (settings->size() > 2 && allowRender) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					if (rectPos.contains(&mousePos) && shouldToggleRightClick) {
						shouldToggleRightClick = false;
						clickMod->isExtended = !clickMod->isExtended;
					}

					GuiUtils::drawCrossLine(vec2_t(
						currentXOffset + windowSize->x + paddingRight - (crossSize / 2) - 1.f,
						currentYOffset + textPadding + (textHeight / 2)
					), MC_Color(1.0f, 0.2f, 0, 1.0f), crossWidth, crossSize, !clickMod->isExtended);

					currentYOffset += textHeight + (textPadding * 2);

					if (clickMod->isExtended) {
						float startYOffset = currentYOffset;
						for (auto it = settings->begin(); it != settings->end(); ++it) {
							SettingEntry* setting = *it;
							if (strcmp(setting->name, "enabled") == 0 || strcmp(setting->name, "keybind") == 0)
								continue;
							
							vec2_t textPos = vec2_t(
								currentXOffset + textPadding + 5,
								currentYOffset + textPadding
							);

							// Incomplete, because we dont know the endY yet
							vec4_t rectPos = vec4_t(
								currentXOffset,
								currentYOffset,
								xEnd,
								0
							);

#ifdef _DEBUG
#ifndef DEBUG_DRAW_SELECTABLE_AREA
//#define DEBUG_DRAW_SELECTABLE_AREA
#endif
#endif


							switch (setting->valueType) {
							case BOOL_T:
							{
								rectPos.w = currentYOffset + textHeight + (textPadding * 2);
								DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f);
								vec4_t selectableSurface = vec4_t(
									textPos.x + textPadding,
									textPos.y + textPadding,
									xEnd - textPadding,
									textPos.y + textHeight - textPadding
								);
								// Debugging
								{
#ifdef DEBUG_DRAW_SELECTABLE_AREA
									DrawUtils::drawRectangle(selectableSurface, MC_Color(1, 0, 0, 1), 0.3f);
#endif
								}
								bool isFocused = selectableSurface.contains(&mousePos);
								// Logic
								{
									if (isFocused && shouldToggleLeftClick) {
										shouldToggleLeftClick = false;
										setting->value->_bool = !setting->value->_bool;
									}
								}
								// Checkbox
								{
									vec4_t boxPos = vec4_t(
										textPos.x              + textPadding,
										textPos.y              + textPadding,
										textPos.x + textHeight - textPadding,
										textPos.y + textHeight - textPadding
									);

									DrawUtils::drawRectangle(boxPos, MC_Color(1, 1, 1, 1), isFocused ? 1 : 0.8f, 0.5f);

									if (setting->value->_bool) {
										DrawUtils::setColor(0.2f, 0.7f, 0.2f, 1);
										boxPos.x += textPadding;
										boxPos.y += textPadding;
										boxPos.z -= textPadding;
										boxPos.w -= textPadding;
										DrawUtils::drawLine(vec2_t(boxPos.x, boxPos.y), vec2_t(boxPos.z, boxPos.w), 0.5f);
										DrawUtils::drawLine(vec2_t(boxPos.z, boxPos.y), vec2_t(boxPos.x, boxPos.w), 0.5f);
									}
								}
								textPos.x += textHeight + (textPadding * 2);
								// Text
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 10 /* because we add 10 to text padding + checkbox*/);
									DrawUtils::drawText(textPos, &elTexto, isFocused ? new MC_Color(1.0f, 1.0f, 1.0f, 1.0f) : new MC_Color(0.8f, 0.8f, 0.8f, 1.0f), textSize);
									currentYOffset += textHeight + (textPadding * 2);
								}
							}
								break;
							case FLOAT_T:
							{
								// Text
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s:", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
									currentYOffset += textPadding + textHeight;
									rectPos.w = currentYOffset;
									DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f);
								}
								// Slider
								{
									vec4_t rect = vec4_t(
										currentXOffset + textPadding + 5,
										currentYOffset + textPadding,
										xEnd - textPadding,
										currentYOffset - textPadding + textHeight
									);
									// Debugging
									{
#ifdef DEBUG_DRAW_SELECTABLE_AREA
										DrawUtils::fillRectangle(rect, MC_Color(1, 0, 0, 1), 0.3f);
#endif
									}
									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPadding * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f); // Background
										DrawUtils::drawRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f, 1.0f), 1.f, 0.7f); // Slider background

										const float minValue = setting->minValue->_float;
										const float maxValue = setting->maxValue->_float - minValue;
										float value = max(0, setting->value->_float - minValue); // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;
										value /= maxValue; // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPadding) - (currentXOffset + textPadding + 5);
										value *= endXlol; // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											vec2_t mid = vec2_t(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 1.5f // Hardcoded ghetto
											);
											char str[10];
											sprintf_s(str, 10, "%.2f", setting->value->_float);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize) / 2;

											DrawUtils::drawText(mid, &text, (areWeFocused || setting->isDragging) ? new MC_Color(1.0f, 0.1f, 0.1f, 1.f) : new MC_Color(0.8f, 0.05f, 0.05f, 1.f), textSize);
										}

										// Draw Progress
										{
											rect.z = rect.x + value;
											DrawUtils::fillRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f, 1.0f), (areWeFocused || setting->isDragging) ? 1.f : 0.8f);
										}

										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown)
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											}
											else if (areWeFocused && shouldToggleLeftClick) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol; // Now in range 0 - 1
											value *= maxValue;
											value += minValue;

											setting->value->_float = value;
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += textHeight + (textPadding * 2);
								}
							}
								break;
							case INT_T:
							{
								// Text
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s:", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
									currentYOffset += textPadding + textHeight;
									rectPos.w = currentYOffset;
									DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f);
								}
								// Slider
								{
									vec4_t rect = vec4_t(
										currentXOffset + textPadding + 5,
										currentYOffset + textPadding,
										xEnd           - textPadding,
										currentYOffset - textPadding + textHeight
									);
									// Debugging
									{
#ifdef DEBUG_DRAW_SELECTABLE_AREA
										DrawUtils::fillRectangle(rect, MC_Color(1, 0, 0, 1), 0.3f);
#endif
									}
									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPadding * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRectangle(rectPos, moduleColor, 0.7f); // Background
										DrawUtils::drawRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f, 1.0f), 1.f, 0.7f); // Slider background

										const float minValue = (float) setting->minValue->_int;
										const float maxValue = (float) setting->maxValue->_int - minValue;
										float value = (float) max(0,   setting->   value->_int - minValue); // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;
										value /= maxValue; // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPadding) - (currentXOffset + textPadding + 5);
										value *= endXlol; // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											vec2_t mid = vec2_t(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 1.5f // Hardcoded ghetto
											);
											char str[10];
											sprintf_s(str, 10, "%i", setting->value->_int);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize) / 2;

											DrawUtils::drawText(mid, &text, (areWeFocused || setting->isDragging) ? new MC_Color(1.0f, 0.1f, 0.1f, 1.f) : new MC_Color(0.8f, 0.05f, 0.05f, 1.f), textSize);
										}

										// Draw Progress
										{
											rect.z = rect.x + value;
											DrawUtils::fillRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f, 1.0f), (areWeFocused || setting->isDragging) ? 1.f : 0.8f);
										}
										
										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown) 
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											}else if (areWeFocused && shouldToggleLeftClick) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol; // Now in range 0 - 1
											value *= maxValue;
											value += minValue;
											
											setting->value->_int = (int) roundf(value);
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += textHeight + (textPadding * 2);
								}
							}
								break;
							default:
							{
								char alc[100];
								sprintf_s(alc, 100, "Not implemented (%s)", setting->name);
								std::string elTexto = alc;
								// Adjust window size if our text is too  t h i c c
								windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);
								
								DrawUtils::drawText(textPos, &elTexto, new MC_Color(1.0f, 1.0f, 1.0f, 1.0f), textSize);
								currentYOffset += textHeight + (textPadding * 2);
							}
								break;
							}
						}
						float endYOffset = currentYOffset;
						if (endYOffset - startYOffset > textHeight + 5) {
							startYOffset += textPadding;
							endYOffset   -= textPadding;
							DrawUtils::setColor(1, 1, 1, 1);
							DrawUtils::drawLine(vec2_t(currentXOffset + 2, startYOffset), vec2_t(currentXOffset + 2, endYOffset), 0.5f);
						}
					}
				}else
					currentYOffset += textHeight + (textPadding * 2);
			}
		}
	}
	DrawUtils::flush();
	// Draw Category Header
	{
		vec2_t textPos = vec2_t(
			currentXOffset + textPadding,
			categoryHeaderYOffset + textPadding
		);
		vec4_t rectPos = vec4_t(
			currentXOffset - categoryMargin,
			categoryHeaderYOffset - categoryMargin,
			currentXOffset + windowSize->x + paddingRight + categoryMargin,
			categoryHeaderYOffset + textHeight + (textPadding * 2)
		);

		// Extend Logic
		{
			if (rectPos.contains(&mousePos) && shouldToggleRightClick && !isDragging) {
				shouldToggleRightClick = false;
				ourWindow->isExtended = !ourWindow->isExtended;
				if (ourWindow->isExtended && ourWindow->animation == 0)
					ourWindow->animation = 0.2f;
				else if (!ourWindow->isExtended && ourWindow->animation == 1)
					ourWindow->animation = 0;
				ourWindow->isInAnimation = true;

				for (std::vector<IModule*>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
					IModule* mod = *it;
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					clickMod->isExtended = false;
				}
			}
		}

		// Dragging Logic
		{
			if (isDragging && getCrcHash(categoryName) == draggedWindow) { // WE are being dragged
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
				draggedWindow = getCrcHash(categoryName);
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
			GuiUtils::drawCrossLine(vec2_t(
				currentXOffset + windowSize->x + paddingRight - (crossSize / 2) - 1.f,
				categoryHeaderYOffset + textPadding + (textHeight / 2)
			), MC_Color(1.0f, 0.2f, 0, 1.0f), crossWidth, crossSize, !ourWindow->isExtended);
		}
	}


	moduleList.clear();
	DrawUtils::flush();
}

void ClickGui::render()
{
	if (!moduleMgr->isInitialized())
		return;

	if (timesRendered < 10)
		timesRendered++;

	// Fill Background
	{
		DrawUtils::fillRectangle(vec4_t(
			0,
			0,
			g_Data.getClientInstance()->getGuiData()->widthGame,
			g_Data.getClientInstance()->getGuiData()->heightGame
		), MC_Color(0.8f, 0.8f, 0.8f, 0.1f), 0.2f);
	}

	// Render all categorys
	renderCategory(Category::COMBAT);
	renderCategory(Category::VISUAL);
	renderCategory(Category::MOVEMENT);
	renderCategory(Category::PLAYER);
	renderCategory(Category::BUILD);
	renderCategory(Category::EXPLOITS);

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

void ClickGui::onKeyUpdate(int key, bool isDown)
{
	static IModule* clickGuiMod = moduleMgr->getModule<ClickGuiMod>();
	if (clickGuiMod == NULL)
		clickGuiMod = moduleMgr->getModule<ClickGuiMod>();
	else {
		if (!isDown)
			return;

		if (!clickGuiMod->isEnabled()) {
			timesRendered = 0;
			return;
		}
		
		if (timesRendered < 10)
			return;
		timesRendered = 0;

		switch (key) {
		case VK_ESCAPE:
			clickGuiMod->setEnabled(false);
			return;
		default:
			if (key == clickGuiMod->getKeybind())
				clickGuiMod->setEnabled(false);
		}
	}
}