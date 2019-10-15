#include "NoFriends.h"

NoFriends::NoFriends() : IModule(0x0, PLAYER)
{
}

NoFriends::~NoFriends()
{
}

const char* NoFriends::getModuleName()
{
	return ("NoFriends");
}

