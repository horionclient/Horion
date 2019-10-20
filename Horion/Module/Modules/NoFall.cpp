#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3, Category::MOVEMENT, "Prevents you from taking falldamage")
{
}


NoFall::~NoFall()
{
}

const char* NoFall::getModuleName()
{
	return ("NoFall");
}

