#include "Bhop.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	registerEnumSetting("Bhop modes", &Bhopmode, 0);
	Bhopmode = SettingEnum(this)
				   .addEntry(EnumEntry("LowhopV2", 0))
				   .addEntry(EnumEntry("Lowhop", 1))
				   .addEntry(EnumEntry("Normal", 2));
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 0.8f);
	registerFloatSetting("Up", &this->up, this->up, 0.1f, 0.6f);
	registerFloatSetting("Boost Amount", &this->Boostnum, this->Boostnum, 0.1f, 0.6f);
	this->registerBoolSetting("Step", &this->step, this->step);
	this->registerBoolSetting("Boost", &this->Boost, this->Boost);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("Bhop");
}
float getspeed;
void Bhop::onEnable() {
	if (Boost)
	getspeed = speed;
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}
void Bhop::onMove(C_MoveInputHandler* input) {
	switch (Bhopmode.selected) {
	default:
		lowhopv2 = true;
		lowhop = false;
		normal = false;
		break;
	case 1:
		lowhopv2 = false;
		lowhop = true;
		normal = false;
		break;
	case 2:
		lowhopv2 = false;
		lowhop = false;
		normal = true;
		break;
	}
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1)
		return;

	if (player->isSneaking())
		return;

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	bool dohop = false;
	if (pressed && player->onGround) {
		dohop = true;
	} else
		dohop = false;
	if (Boost) {
		if (pressed && player->damageTime) {
			speed = getspeed + Boostnum;
		} else {
			speed = getspeed;
		}
	}
	if (normal && dohop) {
		player->jumpFromGround();
	}
	if (lowhop && dohop) {
		player->velocity.y += up;
	}
	if (lowhopv2 && dohop) {
		player->velocity.y += up;
		player->velocity.y -= up / 3;
	}
	if (step) {
		player->stepHeight = 1.f;
	} else
		player->stepHeight = 0.5625f;

	if (!pressed) {
		player->velocity.x *= 0.f;
		player->velocity.z *= 0.f;
	}

	if (pressed) {
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}
void Bhop::onDisable() {
	auto player = g_Data.getLocalPlayer();
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
	g_Data.getLocalPlayer()->stepHeight = 0.5625f;
	player->velocity.x *= 0.f;
	player->velocity.z *= 0.f;
	if (Boost)
	speed = getspeed;
}
