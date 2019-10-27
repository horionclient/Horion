#include "AntiBot.h"

AntiBot::AntiBot() : IModule(0x0, Category::EXPLOITS, "Enable this module to filter bots!")
{
	registerBoolSetting("Hitbox Check", &this->hitboxCheck, this->hitboxCheck);
	registerBoolSetting("Name Check", &this->nameCheck, this->nameCheck);
	registerBoolSetting("Invisible Check", &this->invisibleCheck, this->invisibleCheck);
	registerBoolSetting("EntityID Check", &this->entityIdCheck, this->entityIdCheck);
	registerBoolSetting("Immobile Check", &this->immobileCheck, this->immobileCheck);
}

AntiBot::~AntiBot()
{
}

const char* AntiBot::getModuleName()
{
	return ("AntiBot");
}