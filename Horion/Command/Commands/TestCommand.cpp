#include "TestCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"
#include <sstream>
#include "../../../SDK/Tag.h"

TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "") {
}

TestCommand::~TestCommand() {
}

void listEnts(C_Entity* ent, bool isValid) {
	int id = ent->getEntityTypeId();
	char* name = ent->getNameTag()->getText();
	logF("---------------");
	logF("Entity Name: %s", name);
	logF("Entity ID: %d", id);
	logF("---------------");
}

void showAimedBlockInfo() {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
	char* name = block->toLegacy()->name.getText();
	auto id = block->toLegacy()->blockId;
	g_Data.getClientInstance()->getGuiData()->displayClientMessageF("Block Name: %s", name);
	g_Data.getClientInstance()->getGuiData()->displayClientMessageF("Block ID: %lld", id);
}

bool TestCommand::execute(std::vector<std::string>* args) {
	showAimedBlockInfo();
	return true;
}
