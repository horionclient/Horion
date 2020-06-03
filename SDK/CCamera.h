#pragma once

#include "../Utils/HMath.h"

class C_Camera {
private:
	char padding[0x78]; // 0x0
public:
	bool renderPlayerModel; // 0x078
private:
	char padding79[0x8F]; // 0x079
public:
	bool facesPlayerFront; // 0x108

	void getEulerAngles(vec3_t*);
	void getPlayerRotation(vec2_t*);
	void setOrientation(float pitch, float yaw, float roll);
	void setOrientationDeg(float pitch, float yaw, float roll) {
		setOrientation((-yaw - 180) * RAD_DEG, pitch * RAD_DEG, roll * RAD_DEG);
	}
};