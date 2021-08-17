#include "Freecam.h"

Freecam::Freecam() : IModule('V', Category::MISC, "Move your cam without moving the player") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.50f, 1.25f);
}

Freecam::~Freecam() {
}

const char* Freecam::getModuleName() {
	return ("Freecam");
}

void Freecam::onTick(C_GameMode* gm) {
	gm->player->fallDistance = 0.f;
	gm->player->velocity = vec3_t(0, 0, 0);
}

void Freecam::onEnable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		oldPos = *g_Data.getLocalPlayer()->getPos();
	}
}

void Freecam::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();  
	if (player == nullptr) return;          

	

	vec2_t moveVec2d = { input->forwardMovement, -input->sideMovement };  
	bool pressed = moveVec2d.magnitude() > 0.01f;                       

	float calcYaw = (player->yaw + 90) * (PI / 180);                                    
	vec3_t moveVec;                                                                      
	float c = cos(calcYaw);                                                              
	float s = sin(calcYaw);                                                              
	moveVec2d = { moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c };  
	moveVec.x = moveVec2d.x * speed;                                                     
	moveVec.y = player->velocity.y;                                                      
	moveVec.z = moveVec2d.y * speed;                                                    
	if (pressed) player->lerpMotion(moveVec);                                            
	C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());   
	if (input->isJumping) {
		player->velocity.y += 0.50f;
		
	}
	if (input->isSneakDown) {
		player->velocity.y -= 0.50f;
		
	}
}

void Freecam::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		g_Data.getLocalPlayer()->setPos(oldPos);
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
	}
}
