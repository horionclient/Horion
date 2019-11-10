#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0x0, Category::PLAYER, "Enable this module to filter bots!")
{
	registerBoolSetting("Hitbox Check", &this->hitboxCheck, this->hitboxCheck);
	registerBoolSetting("Name Check", &this->nameCheck, this->nameCheck);
	registerBoolSetting("Invisible Check", &this->invisibleCheck, this->invisibleCheck);
	registerBoolSetting("EntityID Check", &this->entityIdCheck, this->entityIdCheck);
	registerBoolSetting("Other Check", &this->otherCheck, this->otherCheck);
}

AntiBot::~AntiBot()
{
}

const char* AntiBot::getModuleName()
{
	return ("AntiBot");
}