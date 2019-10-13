#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3, MOVEMENT)
{
}


NoFall::~NoFall()
{
}

const char* NoFall::getModuleName()
{
	return ("NoFall");
}

