#include "ImmediateGui.h"

ImmediateGui ImGui;


ComponentInfo::ComponentInfo(int id) : id(id) {
}

ButtonInfo::ButtonInfo(int id, vec2_t pos) : ComponentInfo(id), pos(pos) {
}

void ButtonInfo::calculateSize(const char* txt) {
	this->size = { DrawUtils::getTextWidth(&std::string(txt)), DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight()};
}

bool ButtonInfo::isInSelectableSurface(vec2_t mouse) {
	vec4_t surface = getSelectableSurface();
	return surface.contains(&mouse);
}

vec4_t ButtonInfo::getSelectableSurface() {
	return { this->pos.x,
			 this->pos.y,
			 this->pos.x + this->size.x,
			 this->pos.y + this->size.y };
}

void ImmediateGui::onMouseClickUpdate(int key, bool isDown) {
	switch (key) {
	case 0:  // Left Click
		this->leftMb.nextIsDown = isDown;
		break;
	case 1:  // Right Click
		this->rightMb.nextIsDown = isDown;
		break;
	}
}

void ImmediateGui::startFrame() {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	vec2_t windowSizeReal = g_Data.getClientInstance()->getGuiData()->windowSizeReal;
	this->mousePos = *g_Data.getClientInstance()->getMousePos();
	this->mousePos.div(windowSizeReal);
	this->mousePos.mul(windowSize);

	this->leftMb.update();
	this->rightMb.update();

	if (g_Data.getClientInstance()->getMouseGrabbed()) {
		this->leftMb.isClicked = false;
		this->rightMb.isClicked = false;

		this->mousePos = {-1000, 1000};
	}
}

void ImmediateGui::endFrame() {
	this->leftMb.isClicked = false;
	this->rightMb.isClicked = false;
}

bool ImmediateGui::Button(const char* label, vec2_t pos) {
	auto id = Utils::getCrcHash(label);
	if (this->components.find(id) == this->components.end()) { // Create new button
		this->components[id] = std::make_shared<ButtonInfo>(id, pos);
	}
	auto comp = this->components[id];
	auto button = dynamic_cast<ButtonInfo*>(comp.get());

	button->calculateSize(label);
	DrawUtils::drawText(pos, &std::string(label), MC_Color());
	if (button->isInSelectableSurface(this->mousePos)) { // Mouse hovering over us
		DrawUtils::fillRectangle(button->getSelectableSurface(), MC_Color(0.3f, 0.3f, 0.7f, 1.f), 1);
		if (this->leftMb.trySteal()) { // Click
			return true;
		}
	}else
		DrawUtils::fillRectangle(button->getSelectableSurface(), MC_Color(0.1f, 0.1f, 0.7f, 1.f), 1);
		

	return false;
}

