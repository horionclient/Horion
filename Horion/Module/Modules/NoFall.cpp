#include "NoFall.h"



NoFall::NoFall() : IModule(VK_NUMPAD3)
{
}


NoFall::~NoFall()
{
}

std::string NoFall::getModuleName()
{
	return std::string("NoFall");
}

