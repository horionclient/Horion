#include "ExtendedBlockReach.h"

ExtendedBlockReach::ExtendedBlockReach() : IModule(0x0, Category::PLAYER, "Exteneded reach for placing/breaking blocks") {
	this->registerFloatSetting(std::string("reach"), &this->blockReach, this->blockReach, 7, 20);
}

ExtendedBlockReach::~ExtendedBlockReach() {
}

const char* ExtendedBlockReach::getModuleName() {
	return ("ExtendedBlockReach");
}
