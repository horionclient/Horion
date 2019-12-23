#include "Step.h"



Step::Step() : IModule('B', Category::MOVEMENT, "Increases your step height") {
	registerFloatSetting("height", &this->height, this->height, 1.f, 10.f);
}


Step::~Step() {
}

const char* Step::getModuleName() {
	return ("Step");
}

void Step::onTick(C_GameMode* gm) {
	if(gm->player != nullptr)
		gm->player->stepHeight = height;
}
void Step::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) 
		g_Data.getLocalPlayer()->stepHeight = 0.5625f;	
}