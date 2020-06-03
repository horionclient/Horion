#include "CCamera.h"

#include "../Utils/Utils.h"

void C_Camera::getEulerAngles(vec3_t* vec) {
	using cam_getEuler_t = void(__fastcall*)(C_Camera*, vec3_t*);
	static auto cam_getEuler = reinterpret_cast<cam_getEuler_t>(FindSignature("48 8B C4 53 48 81 EC ?? ?? ?? ?? 0F 29 70 ?? 48 8B DA 0F 29 78 ?? F3"));
	cam_getEuler(this, vec);
}
void C_Camera::setOrientation(float pitch, float yaw, float roll) {
	using cam_setOrientation_t = void(__fastcall*)(C_Camera*, float, float, float);
	static auto cam_setOrientation = reinterpret_cast<cam_setOrientation_t>(FindSignature("40 53 48 81 EC ?? ?? ?? ?? 0F 29 74 24 ?? 0F 57"));
	cam_setOrientation(this, pitch, yaw, roll);
}
void C_Camera::getPlayerRotation(vec2_t* angOut) {
	vec3_t angles;
	this->getEulerAngles(&angles);

	float v4;
	if ( this->facesPlayerFront )
		v4 = -1.0;
	else
		v4 = 1.0;

	angOut->y = (float)(angles.y * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 272);
	angOut->x = (float)((float)(angles.x * 57.295776) + *(float *)(reinterpret_cast<__int64>(this) + 268)) * v4;
	angOut->normAngles();
}
