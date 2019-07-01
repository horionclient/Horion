#include "NoSlowDown.h"



NoSlowDown::NoSlowDown() : IModule(0x0, MOVEMENT)
{
}


NoSlowDown::~NoSlowDown()
{
}

const char* NoSlowDown::getModuleName()
{
	return ("NoSlowDown");
}

void NoSlowDown::onTick(C_GameMode* gm) 
{
}