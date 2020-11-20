#include "Strafe.h"

Strafe::Strafe() : IModule(0x0, Category::MOVEMENT, "Bhop Without the Jump, Can be used as some kind of Speedhack.") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 1.f);
}

Strafe::~Strafe() {
}

const char* Strafe::getModuleName() {
	return ("Strafe");
}

void Strafe::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1) 
		return;

	if (player->isSneaking()) 
		return;

	float yaw = player->yaw;

	bool pressed = input->forward || input->backward || input->right || input->left;

	if (input->forward && input->backward)
		return;
	
	if (input->right) {
		yaw += 90.f;
		if (input->forward)
			yaw -= 45.f;
		else if (input->backward)
			yaw += 45.f;
	}
	if (input->left) {
		yaw -= 90.f;
		if (input->forward)
			yaw += 45.f;
		else if (input->backward)
			yaw -= 45.f;
	}

	if (input->backward && !input->left && !input->right)
		yaw += 180.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	vec3_t moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if(pressed) player->lerpMotion(moveVec);
}
