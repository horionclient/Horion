#include "Tower.h"



Tower::Tower() : IModule(VK_NUMPAD9, EXPLOITS)
{
}


Tower::~Tower()
{
}

const char* Tower::getModuleName()
{
	return ("Tower");
}

void Tower::onEnable() {

	this->delay = 0;

}

void Tower::onTick(C_GameMode* gm) {

	this->delay++;

	int x = g_Data.getLocalPlayer()->aabb.lower.x;
	int y = g_Data.getLocalPlayer()->aabb.lower.y;
	int z = g_Data.getLocalPlayer()->aabb.lower.z;

}

void Tower::onDisable() {

	this->delay = 0;

}
