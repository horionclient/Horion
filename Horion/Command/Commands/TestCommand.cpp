#include "TestCommand.h"
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
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(pointingStruct->block);
	int id = block->toLegacy()->blockId;
	char* name = block->toLegacy()->name.getText();
	logF("---------------");
	logF("Block Name: %s", name);
	logF("Block ID: %d", id);
	logF("---------------");
}

bool TestCommand::execute(std::vector<std::string>* args) {
	using give = __int64*(__fastcall*)(void**, void*, TextHolder);
	void* ah = malloc(0x10);
	void* h = malloc(0x100);
	give func = (give)(0x7FF655094660);
	__int64* a = func(&ah, h, TextHolder("beehive"));
	free(h);
	logF("%llX", a);
	logF("2 : %llX", ah);
	return true;
} 
