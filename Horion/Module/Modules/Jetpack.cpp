#include "Jetpack.h"



Jetpack::Jetpack() : IModule('F')
{
}


Jetpack::~Jetpack()
{
}

bool Jetpack::isFlashMode() {
	return true;
}

std::string Jetpack::getModuleName()
{
	return std::string("Jetpack");
}


void Jetpack::onTick(C_GameMode * gm)
{
	float calcYaw = (gm->player->yaw + 90) *  (PI / 180);
	float calcPitch = (gm->player->pitch)  * -(PI / 180);

	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * cos(calcPitch);
	moveVec.y = sin(calcPitch)				 ;
	moveVec.z = sin(calcYaw) * cos(calcPitch);
	gm->player->setVelocity(moveVec);
}
