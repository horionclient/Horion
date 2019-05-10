#include "Step.h"



Step::Step() : IModule('B')
{
}


Step::~Step()
{
}

std::string Step::getModuleName()
{
	return std::string("Step");
}

void Step::onTick(C_GameMode* gm) {
	if(gm->player != nullptr)
		gm->player->stepHeight = 10;
}
void Step::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) 
		g_Data.getLocalPlayer()->stepHeight = 0.5625f;
	
}