#include "TestModule.h"



TestModule::TestModule() : IModule(0, Category::EXPLOITS, "For testing purposes") {
	
}


TestModule::~TestModule() {
}

const char* TestModule::getModuleName() {
	return "TestModule";
}

void TestModule::onTick(C_GameMode* gm) {
	
}
void TestModule::onDisable() {
	
}