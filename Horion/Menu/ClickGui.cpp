#include "ClickGui.h"

bool isLeftClickDown = false;
bool isRightClickDown = false;
bool shouldToggleLeftClick = false;  // If true, toggle the focused module
bool shouldToggleRightClick = false;
bool resetStartPos = true;
bool initialised = false;

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
static constexpr float backgroundAlpha = 1;
static const MC_Color selectedModuleColor = MC_Color(28, 107, 201);
static const MC_Color moduleColor = MC_Color(13, 29, 48);

float currentYOffset = 0;
float currentXOffset = 0;

int timesRendered = 0;

void ClickGui::getModuleListByCategory(Category category, std::vector<std::shared_ptr<IModule>>* modList) {
	auto lock = moduleMgr->lockModuleList();
	std::vector<std::shared_ptr<IModule>>* moduleList = moduleMgr->getModuleList();

	for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList->begin(); it != moduleList->end(); ++it) {
		if ((*it)->getCategory() == category)
			modList->push_back(*it);
	}
}

std::shared_ptr<ClickWindow> ClickGui::getWindow(const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = windowMap.find(id);
	if (search != windowMap.end()) {  // Window exists already
		return search->second;
	} else {  // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickWindow> newWindow = std::make_shared<ClickWindow>();

		windowMap.insert(std::make_pair(id, newWindow));
		return newWindow;
	}
}

std::shared_ptr<ClickModule> ClickGui::getClickModule(std::shared_ptr<ClickWindow> window, const char* name) {
	unsigned int id = Utils::getCrcHash(name);

	auto search = window->moduleMap.find(id);
	if (search != window->moduleMap.end()) {  // Window exists already
		return search->second;
	} else {  // Create window
		// TODO: restore settings for position etc
		std::shared_ptr<ClickModule> newModule = std::make_shared<ClickModule>();

		window->moduleMap.insert(std::make_pair(id, newModule));
		return newModule;
	}
}

void ClickGui::renderLabel(const char* text) {
}

void ClickGui::renderTooltip(std::string* text) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t currentTooltipPos = vec2_t(5.f, windowSize.y - 15.f);
	float textWidth = DrawUtils::getTextWidth(text, textSize);
	vec2_t textPos = vec2_t(
		currentTooltipPos.x + textPadding,
		currentTooltipPos.y);
	vec4_t rectPos = vec4_t(
		currentTooltipPos.x - 2.f,
		currentTooltipPos.y - 2.f,
		currentTooltipPos.x + (textPadding * 2) + textWidth + 2.f,
		currentTooltipPos.y + textHeight + 2.f);
	DrawUtils::fillRectangle(rectPos, MC_Color(13, 29, 48), 1.0f);
	DrawUtils::drawRectangle(rectPos, MC_Color(255, 255, 255), 1.f, 0.5f);
	DrawUtils::drawText(textPos, text, MC_Color(255, 255, 255), textSize);
}

void ClickGui::renderCategory(Category category) {
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
		case Category::WORLD:
			categoryName = "World";
			break;
		case Category::MISC:
			categoryName = "Misc";
			break;
		}
	}

	const std::shared_ptr<ClickWindow> ourWindow = getWindow(categoryName);

	// Reset Windows to pre-set positions to avoid confusion
	if (resetStartPos) {
		ourWindow->pos.y = 4;
		switch (category) {
		case Category::COMBAT:
			ourWindow->pos.x = 75;
			break;
		case Category::VISUAL:
			ourWindow->pos.x = 150;
			break;
		case Category::MOVEMENT:
			ourWindow->pos.x = 225;
			break;
		case Category::PLAYER:
			ourWindow->pos.x = 315;
			break;
		case Category::WORLD:
			ourWindow->pos.x = 415;
			break;
		case Category::MISC:
			ourWindow->pos.x = 490;
			break;
		}
	}

	const float xOffset = ourWindow->pos.x;
	const float yOffset = ourWindow->pos.y;
	vec2_t* windowSize = &ourWindow->size;
	currentXOffset = xOffset;
	currentYOffset = yOffset;

	// Get All Modules in our category
	std::vector<std::shared_ptr<IModule>> moduleList;
	getModuleListByCategory(category, &moduleList);

	// Get max width of all text
	{
		for (auto it = moduleList.begin(); it != moduleList.end(); ++it) {
			std::string label = (*it)->getModuleName();
			windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&label, textSize, Fonts::SMOOTH));
		}
	}

	const float xEnd = currentXOffset + windowSize->x + paddingRight;

	vec2_t mousePos = *g_Data.getClientInstance()->getMousePos();

	// Convert mousePos to visual Pos
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
		vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
		
		mousePos.div(windowSizeReal);
		mousePos.mul(windowSize);
	}

	float categoryHeaderYOffset = currentYOffset;

	if (ourWindow->isInAnimation) {
		if (ourWindow->isExtended) {
			ourWindow->animation *= 0.85f;
			if (ourWindow->animation < 0.001f)
				ourWindow->isInAnimation = false;
		} else {
			ourWindow->animation = 1 - ((1 - ourWindow->animation) * 0.85f);
			if (1 - ourWindow->animation < 0.001f)
				ourWindow->isInAnimation = false;
		}
	}

	currentYOffset += textHeight + (textPadding * 2);
	// Loop through Modules to display em
	if (ourWindow->isExtended || ourWindow->isInAnimation) {
		if (ourWindow->isInAnimation) {
			currentYOffset -= ourWindow->animation * moduleList.size() * (textHeight + (textPadding * 2));
		}

		for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
			auto mod = *it;
			std::string textStr = mod->getModuleName();

			vec2_t textPos = vec2_t(
				currentXOffset + textPadding,
				currentYOffset + textPadding);
			vec4_t rectPos = vec4_t(
				currentXOffset,
				currentYOffset,
				xEnd,
				currentYOffset + textHeight + (textPadding * 2));

			bool allowRender = currentYOffset >= categoryHeaderYOffset;

			// Background
			if (allowRender) {
				if (!ourWindow->isInAnimation && !isDragging && rectPos.contains(&mousePos)) {  // Is the Mouse hovering above us?
					DrawUtils::fillRectangle(rectPos, selectedModuleColor, backgroundAlpha);
					std::string tooltip = mod->getTooltip();
					static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>();
					if (clickGuiMod->showTooltips && tooltip.size() > 0)
						renderTooltip(&tooltip);
					if (shouldToggleLeftClick && !ourWindow->isInAnimation) {  // Are we being clicked?
						mod->toggle();
						shouldToggleLeftClick = false;
					}
				} else {
					DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);
				}
			}

			// Text
			if (allowRender)
				DrawUtils::drawText(textPos, &textStr, mod->isEnabled() ? MC_Color(255, 255, 255) : MC_Color(200, 200, 200), textSize);

			// Settings
			{
				std::vector<SettingEntry*>* settings = mod->getSettings();
				if (settings->size() > 2 && allowRender) {
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					if (rectPos.contains(&mousePos) && shouldToggleRightClick && !ourWindow->isInAnimation) {
						shouldToggleRightClick = false;
						clickMod->isExtended = !clickMod->isExtended;
					}

					GuiUtils::drawCrossLine(vec2_t(
												currentXOffset + windowSize->x + paddingRight - (crossSize / 2) - 1.f,
												currentYOffset + textPadding + (textHeight / 2)),
											MC_Color(255, 255, 255), crossWidth, crossSize, !clickMod->isExtended);

					currentYOffset += textHeight + (textPadding * 2);

					if (clickMod->isExtended) {
						float startYOffset = currentYOffset;
						for (auto it = settings->begin(); it != settings->end(); ++it) {
							SettingEntry* setting = *it;
							if (strcmp(setting->name, "enabled") == 0 || strcmp(setting->name, "keybind") == 0)
								continue;

							vec2_t textPos = vec2_t(
								currentXOffset + textPadding + 5,
								currentYOffset + textPadding);

							// Incomplete, because we dont know the endY yet
							vec4_t rectPos = vec4_t(
								currentXOffset,
								currentYOffset,
								xEnd,
								0);

							switch (setting->valueType) {
							case ValueType::BOOL_T: {
								rectPos.w = currentYOffset + textHeight + (textPadding * 2);
								DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);
								vec4_t selectableSurface = vec4_t(
									textPos.x + textPadding,
									textPos.y + textPadding,
									xEnd - textPadding,
									textPos.y + textHeight - textPadding);

								bool isFocused = selectableSurface.contains(&mousePos);
								// Logic
								{
									if (isFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
										shouldToggleLeftClick = false;
										setting->value->_bool = !setting->value->_bool;
									}
								}
								// Checkbox
								{
									vec4_t boxPos = vec4_t(
										textPos.x + textPadding,
										textPos.y + textPadding,
										textPos.x + textHeight - textPadding,
										textPos.y + textHeight - textPadding);

									DrawUtils::drawRectangle(boxPos, MC_Color(255, 255, 255), isFocused ? 1 : 0.8f, 0.5f);

									if (setting->value->_bool) {
										DrawUtils::setColor(28, 107, 201, 1);
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
									DrawUtils::drawText(textPos, &elTexto, isFocused ? MC_Color(1.0f, 1.0f, 1.0f) : MC_Color(0.8f, 0.8f, 0.8f), textSize);
									currentYOffset += textHeight + (textPadding * 2);
								}
							} break;
							case ValueType::FLOAT_T: {
								// Text and background
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s:", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, MC_Color(1.0f, 1.0f, 1.0f), textSize);
									currentYOffset += textPadding + textHeight;
									rectPos.w = currentYOffset;
									DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);
								}
								// Slider
								{
									vec4_t rect = vec4_t(
										currentXOffset + textPadding + 5,
										currentYOffset + textPadding,
										xEnd - textPadding,
										currentYOffset - textPadding + textHeight);

									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPadding * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);              // Background
										DrawUtils::drawRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f), 1.f, backgroundAlpha);  // Slider background

										const float minValue = setting->minValue->_float;
										const float maxValue = setting->maxValue->_float - minValue;
										float value = max(0, setting->value->_float - minValue);  // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;
										value /= maxValue;  // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPadding) - (currentXOffset + textPadding + 5);
										value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											vec2_t mid = vec2_t(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 0.2f
											);
											char str[10];
											sprintf_s(str, 10, "%.2f", setting->value->_float);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize) / 2;

											DrawUtils::drawText(mid, &text, MC_Color(255, 255, 255), textSize);
										}

										// Draw Progress
										{
											rect.z = rect.x + value;
											DrawUtils::fillRectangle(rect, MC_Color(28, 107, 201), (areWeFocused || setting->isDragging) ? 1.f : 0.8f);
										}

										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown)
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											} else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol;  // Now in range 0 - 1
											value *= maxValue;
											value += minValue;

											setting->value->_float = value;
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += textHeight + (textPadding * 2);
								}
							} break;
							case ValueType::INT_T: {

								
								// Text and background
								{
									// Convert first letter to uppercase for more friendlieness
									char name[0x21];
									sprintf_s(name, 0x21, "%s:", setting->name);
									if (name[0] != 0)
										name[0] = toupper(name[0]);

									std::string elTexto = name;
									windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);
									DrawUtils::drawText(textPos, &elTexto, MC_Color(1.0f, 1.0f, 1.0f), textSize);
									currentYOffset += textPadding + textHeight;
									rectPos.w = currentYOffset;
									DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);
								}
								// Slider
								{
									vec4_t rect = vec4_t(
										currentXOffset + textPadding + 5,
										currentYOffset + textPadding,
										xEnd - textPadding,
										currentYOffset - textPadding + textHeight);

									// Visuals & Logic
									{
										rectPos.y = currentYOffset;
										rectPos.w += textHeight + (textPadding * 2);
										// Background
										const bool areWeFocused = rect.contains(&mousePos);

										DrawUtils::fillRectangle(rectPos, moduleColor, backgroundAlpha);              // Background
										DrawUtils::drawRectangle(rect, MC_Color(1.0f, 1.0f, 1.0f), 1.f, backgroundAlpha);  // Slider background

										const float minValue = (float)setting->minValue->_int;
										const float maxValue = (float)setting->maxValue->_int - minValue;
										float value = (float)max(0, setting->value->_int - minValue);  // Value is now always > 0 && < maxValue
										if (value > maxValue)
											value = maxValue;
										value /= maxValue;  // Value is now in range 0 - 1
										const float endXlol = (xEnd - textPadding) - (currentXOffset + textPadding + 5);
										value *= endXlol;  // Value is now pixel diff between start of bar and end of progress

										// Draw Int
										{
											vec2_t mid = vec2_t(
												rect.x + ((rect.z - rect.x) / 2),
												rect.y - 0.2f  // Hardcoded ghetto
											);
											char str[10];
											sprintf_s(str, 10, "%i", setting->value->_int);
											std::string text = str;
											mid.x -= DrawUtils::getTextWidth(&text, textSize) / 2;

											DrawUtils::drawText(mid, &text, MC_Color(255, 255, 255), textSize);
										}

										// Draw Progress
										{
											rect.z = rect.x + value;
											DrawUtils::fillRectangle(rect, MC_Color(28, 107, 201), (areWeFocused || setting->isDragging) ? 1.f : 0.8f);
										}

										// Drag Logic
										{
											if (setting->isDragging) {
												if (isLeftClickDown && !isRightClickDown)
													value = mousePos.x - rect.x;
												else
													setting->isDragging = false;
											} else if (areWeFocused && shouldToggleLeftClick && !ourWindow->isInAnimation) {
												shouldToggleLeftClick = false;
												setting->isDragging = true;
											}
										}

										// Save Value
										{
											value /= endXlol;  // Now in range 0 - 1
											value *= maxValue;
											value += minValue;

											setting->value->_int = (int)roundf(value);
											setting->makeSureTheValueIsAGoodBoiAndTheUserHasntScrewedWithIt();
										}
									}
									currentYOffset += textHeight + (textPadding * 2);
								}
							} break;
							default: {
								char alc[100];
								sprintf_s(alc, 100, "Not implemented (%s)", setting->name);
								std::string elTexto = alc;
								// Adjust window size if our text is too  t h i c c
								windowSize->x = max(windowSize->x, DrawUtils::getTextWidth(&elTexto, textSize) + 5 /* because we add 5 to text padding*/);

								DrawUtils::drawText(textPos, &elTexto, MC_Color(1.0f, 1.0f, 1.0f), textSize);
								currentYOffset += textHeight + (textPadding * 2);
							} break;
							}
						}
						float endYOffset = currentYOffset;
						if (endYOffset - startYOffset > textHeight + 5) {
							startYOffset += textPadding;
							endYOffset -= textPadding;
							DrawUtils::setColor(1, 1, 1, 1);
							DrawUtils::drawLine(vec2_t(currentXOffset + 2, startYOffset), vec2_t(currentXOffset + 2, endYOffset), 0.5f);
						}
					}
				} else
					currentYOffset += textHeight + (textPadding * 2);
			}
		}
	}
	DrawUtils::flush();
	// Draw Category Header
	{
		vec2_t textPos = vec2_t(
			currentXOffset + textPadding,
			categoryHeaderYOffset + textPadding);
		vec4_t rectPos = vec4_t(
			currentXOffset - categoryMargin,
			categoryHeaderYOffset - categoryMargin,
			currentXOffset + windowSize->x + paddingRight + categoryMargin,
			categoryHeaderYOffset + textHeight + (textPadding * 2));

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

				for (std::vector<std::shared_ptr<IModule>>::iterator it = moduleList.begin(); it != moduleList.end(); ++it) {
					auto mod = *it;
					std::shared_ptr<ClickModule> clickMod = getClickModule(ourWindow, mod->getRawModuleName());
					clickMod->isExtended = false;
				}
			}
		}

		// Dragging Logic
		{
			if (isDragging && Utils::getCrcHash(categoryName) == draggedWindow) {  // WE are being dragged
				if (isLeftClickDown) {                                      // Still dragging
					vec2_t diff = vec2_t(mousePos).sub(dragStart);
					ourWindow->pos.add(diff);
					dragStart = mousePos;
				} else {  // Stopped dragging
					isDragging = false;
				}
			} else if (rectPos.contains(&mousePos) && shouldToggleLeftClick) {
				isDragging = true;
				draggedWindow = Utils::getCrcHash(categoryName);
				shouldToggleLeftClick = false;
				dragStart = mousePos;
			}
		}

		// Draw component
		{
			// Draw Text
			std::string textStr = categoryName;
			DrawUtils::drawText(textPos, &textStr, MC_Color(255, 255, 255), textSize);
			DrawUtils::fillRectangle(rectPos, moduleColor, 1.f);
			
			DrawUtils::fillRectangle(vec4_t(rectPos.x, rectPos.w - 1, rectPos.z, rectPos.w), selectedModuleColor, 1 - ourWindow->animation);
			// Draw Dash
			GuiUtils::drawCrossLine(vec2_t(
										currentXOffset + windowSize->x + paddingRight - (crossSize / 2) - 1.f,
										categoryHeaderYOffset + textPadding + (textHeight / 2)),
									MC_Color(255, 255, 255), crossWidth, crossSize, !ourWindow->isExtended);
		}
	}

	// anti idiot
	{
		vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

		if (ourWindow->pos.x + ourWindow->size.x > windowSize.x) {
			ourWindow->pos.x = windowSize.x - ourWindow->size.x;
		}

		if (ourWindow->pos.y + ourWindow->size.y > windowSize.y) {
			ourWindow->pos.y = windowSize.y - ourWindow->size.y;
		}

		ourWindow->pos.x = max(0, ourWindow->pos.x);
		ourWindow->pos.y = max(0, ourWindow->pos.y);
	}

	moduleList.clear();
	DrawUtils::flush();
}

void ClickGui::render() {
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
									 g_Data.getClientInstance()->getGuiData()->heightGame),
								 MC_Color(33, 34, 48), 0.2f);
	}

	// Render all categorys
	renderCategory(Category::COMBAT);
	renderCategory(Category::VISUAL);
	renderCategory(Category::MOVEMENT);
	renderCategory(Category::PLAYER);
	renderCategory(Category::WORLD);
	renderCategory(Category::MISC);

	shouldToggleLeftClick = false;
	shouldToggleRightClick = false;
	resetStartPos = false;

	DrawUtils::flush();
}

void ClickGui::init() { initialised = true; }

void ClickGui::onMouseClickUpdate(int key, bool isDown) {
	switch (key) {
	case 0:  // Left Click
		isLeftClickDown = isDown;
		shouldToggleLeftClick = isDown;
		break;
	case 1:  // Right Click
		isRightClickDown = isDown;
		shouldToggleRightClick = isDown;
		break;
	}
}

void ClickGui::onKeyUpdate(int key, bool isDown) {
	if (!initialised)
		return;
	static auto clickGuiMod = moduleMgr->getModule<ClickGuiMod>();
	
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
