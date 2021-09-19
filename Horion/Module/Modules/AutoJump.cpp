#include "AutoJump.h"

AutoJump::AutoJump() : IModule(0, Category::MOVEMENT, "Automatically jump") {
}

AutoJump::~AutoJump() {}

const char* AutoJump::getModuleName() {
	return ("AutoJump");
}

void AutoJump::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();

	if (player->onGround) gm->player->jumpFromGround();
}