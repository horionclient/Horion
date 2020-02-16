#pragma once

#include <map>
#include "../Utils/Utils.h"
#include "../Utils/HMath.h"
#include "DrawUtils.h"

class ComponentInfo {
public:
	ComponentInfo(int id);
	virtual ~ComponentInfo(){};

protected:
	unsigned int id;
};

class ButtonInfo : public ComponentInfo {
private:
	vec2_t pos;
	vec2_t size;

public:
	ButtonInfo(int id, vec2_t pos);
	virtual ~ButtonInfo(){};

	void calculateSize(const char*);
	bool isInSelectableSurface(vec2_t mouse);
	vec4_t getSelectableSurface();

	bool wasClicked = false;
};

struct KeyInfo {
	bool isDown; // Held down right now
	bool isClicked; // Did it go down this frame
	bool nextIsDown;

	void update() {
		if (!isDown && nextIsDown)
			isClicked = true;
		isDown = nextIsDown;
	}

	bool trySteal() {
		if (isClicked) {
			isClicked = false;
			return true;
		}
		return false;
	}
};

class ImmediateGui {
private:
	vec2_t mousePos;
	KeyInfo leftMb;
	KeyInfo rightMb;
	std::map<unsigned int, std::shared_ptr<ComponentInfo>> components;

public:
	void onMouseClickUpdate(int key, bool isDown);
	void startFrame();
	void endFrame();
	bool Button(const char* label, vec2_t pos);
};

extern ImmediateGui ImGui;