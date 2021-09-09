#include "BlockFly.h"

BlockFly::BlockFly() : IModule(0, Category::CUSTOM, "Block Fly") {
	registerFloatSetting("Speed", &this->speed, this->speed, 0.1f, 0.7f);
	registerIntSetting("Timer Speed", &this->timer, this->timer, 20, 200);
	registerBoolSetting("Timer", &this->time, this->time);
	registerBoolSetting("Damage", &this->dmg, this->dmg);
	registerBoolSetting("Fail Safe", &this->safe, this->safe);
}

BlockFly::~BlockFly() {
}

const char* BlockFly::getModuleName() {
	return ("BlockFly");
}

void BlockFly::onEnable() {
	if (this->dmg) {
		auto player = g_Data.getLocalPlayer();
		player->animateHurt();
	}
}

void BlockFly::onMove(C_MoveInputHandler* input) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (player->isInLava() == 1 || player->isInWater() == 1)
		return;

	if (player->isSneaking())
		return;

	vec2_t moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	if (!player->onGround) {
		g_Data.getGuiData()->displayClientMessageF("[%sBlockFly%s] %sU Cant Start In The Air Idiot%s!", BLUE, WHITE, BLUE, WHITE, configMgr, WHITE);
		if (this->safe) {
			player->velocity.y = 0.00f;
			player->velocity.y = 0.00f;
			player->velocity.y = 0.00f;
			auto scaff = moduleMgr->getModule<Scaffold>();
			scaff->setEnabled(true);
		}
		this->setEnabled(false);
		return;
	}
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	//if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock())) {  // Block in hand?
		//clientMessageF("U Have No Blocks In Ur Inventory :/");
		//this->setEnabled(false);
	//}
	if (pressed) {
		float calcYaw = (player->yaw + 90) * (PI / 180);
		vec3_t moveVec;
		float c = cos(calcYaw);
		float s = sin(calcYaw);
		moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = 0.00f;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}

	if (pressed && counter >= 0) {
		auto blinkMod = moduleMgr->getModule<Scaffold>();
		if (blinkMod->isEnabled()) {
			blinkMod->setEnabled(false);
		}
	}
	if (pressed && counter == 6) {
		auto blinkMod = moduleMgr->getModule<Scaffold>();
		if (this->Fly) {
			blinkMod->setEnabled(true);
		}
	}
	if (pressed && counter == 4) {
		//auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
		//if (!selectedItem->isValid() || !(*selectedItem->item)->isBlock())  // Block in hand?
			//return;
		player->velocity.x = 0.f;
		player->velocity.y = 0.00f;
		player->velocity.z = 0.f;
		//auto player = g_Data.getLocalPlayer();
		//player->jumpFromGround();
	}
	if (pressed && counter == 5) {
		player->velocity.x = 0.f;
		player->velocity.y = 0.00f;
		player->velocity.z = 0.f;
	}
	if (pressed && counter == 6) {
		player->velocity.x = 0.f;
		player->velocity.y = 0.0f;
		player->velocity.z = 0.f;
	}
	if (pressed && counter == 7) {
		player->velocity.x = 0.f;
		player->velocity.y = -0.01f;
		player->velocity.z = 0.f;
	}
	if (pressed && counter == 8) {
		player->velocity.x = 0.f;
		player->velocity.y = 0.00f;
		player->velocity.z = 0.f;
	}

	if (counter == 11) {
		counter = 0;
	} else {
		counter++;
	}
	if (timer) {
		*g_Data.getClientInstance()->minecraft->timer = static_cast<float>(this->timer);
	} else {
		*g_Data.getClientInstance()->minecraft->timer = 20.f;
	}
}
void BlockFly::onTick(C_GameMode* gm) {
}

void BlockFly::onDisable() {
	auto blinkMod = moduleMgr->getModule<Scaffold>();
	auto player = g_Data.getLocalPlayer();
	blinkMod->spoof = true;
	if (this->safe) {
		//hi :)
	} else {
		if (blinkMod->isEnabled()) {
			blinkMod->setEnabled(false);
		}
	}
	*g_Data.getClientInstance()->minecraft->timer = 20.f;
}