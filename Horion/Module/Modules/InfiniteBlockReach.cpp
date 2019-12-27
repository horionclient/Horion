#include "InfiniteBlockReach.h"



InfiniteBlockReach::InfiniteBlockReach() : IModule(0x0, Category::PLAYER, "Infinite reach for placing/breaking blocks")
{
	this->registerFloatSetting(std::string("reach"), &this->blockReach, this->blockReach, 7, 100);
}


InfiniteBlockReach::~InfiniteBlockReach()
{
}

const char* InfiniteBlockReach::getModuleName()
{
	return ("InfiniteBlockReach");
}