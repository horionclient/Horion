#pragma once

#include "../Utils/HMath.h"

class C_Camera {
private:
	char padding[0x78]; // 0x0
public:
	bool renderPlayerModel; // 0x078
	bool renderFirstPersonObjects; // 0x079 probably hand and item
private:
	char autoAlignPadding[2]; // 0x07A
	char padding7C[0x18]; // 0x07C
public:
	float fieldOfView; // 0x094
	float nearClippingPlane; // 0x098 usually 0.025
	float farClippingPlane; // 0x09C usually 2500
private:
	char paddingA0[0x68]; // 0x0A0
public:
	bool facesPlayerFront; // 0x108

	void getForwardVector(vec3_t*);
	void getEulerAngles(vec3_t*);
	void getPlayerRotation(vec2_t*);
	void setOrientation(float pitch, float yaw, float roll);
	void setOrientationDeg(float pitch, float yaw, float roll) {
		setOrientation((-yaw - 180) * RAD_DEG, -pitch * RAD_DEG, roll * RAD_DEG);
	}
};

class C_CameraManager {
public:
	C_Camera* getCameraOrDebugCamera();
};