#include "TestCommand.h"


TestCommand::TestCommand() : IMCCommand("test", "Test for Debugging purposes", "")
{
}


TestCommand::~TestCommand()
{
}

bool TestCommand::execute(std::vector<std::string>* args)
{
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (!Target::isValidTarget(ent)) return;
		const char* name = ent->getNameTag()->getText();
		int id = ent->getEntityTypeId();
		std::string hitbox = std::to_string(ent->height) + ", " + std::to_string(ent->width);
		g_Data.getGuiData()->displayClientMessageF("Name: %s\nID: %s\nHitbox: %s", name, std::to_string(id).c_str(), hitbox.c_str()); 
		// logF("Name: %s", ent->getNameTag()->getText());
		});
	return true;
}