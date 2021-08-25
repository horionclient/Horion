#include "ExtendedBlockReach.h"

ExtendedBlockReach::ExtendedBlockReach() : IModule(0, Category::PLAYER, "Exteneded reach for placing/breaking blocks") {
	registerFloatSetting(std::string("reach"), &blockReach, blockReach, 7, 20);
}

ExtendedBlockReach::~ExtendedBlockReach() {
}

const char* ExtendedBlockReach::getModuleName() {
	return ("ExtendedBlockReach");
}
