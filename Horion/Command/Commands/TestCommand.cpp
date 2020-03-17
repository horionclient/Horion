#include "TestCommand.h"

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
	std::string h = Utils::getClipboardText();
	//logF("%s", h.c_str());
	g_Data.getClientInstance()->getGuiData()->displayClientMessage(&h);
	CompoundTag* tag = new CompoundTag();
	g_Data.getClientInstance()->getPointerStruct()->entityPtr->save(tag);
	//logF("item %llX", tag);

	//std::string h = "{Occupants:{TicksLeftToStay:1,ActorIdentifier:\"minecraft:command_block_minecart<>\",SaveData:{UniqueID:545l,Saddled:0b,Invulnerable:0b,LastDimensionId:0,SuccessCount:0,IsStunned:0b,FallDistance:0.0f,ExecuteOnFirstTick:1b,Version:10,IsEating:0b,TickDelay:3,definitions:[\"+minecraft:command_block_minecart\",\"+minecraft:command_block_inactive\"],CurrentTickCount:0,ShowBottom:0b,identifier:\"minecraft:command_block_minecart\",IsTamed:0b,LastExecution:0l,Color:0b,ChestItems:[{Slot:0b,Count:0b,Damage:0s,Name:\"\"}],IsOrphaned:0b,IsRoaring:0b,LootDropped:0b,IsScared:0b,IsIllagerCaptain:0b,Variant:0,CustomName:\"\",Pos:[640.519f,66.35f,32.207695f],Chested:0b,IsTrusting:0b,IsGliding:0b,StrengthMax:0,IsBaby:0b,Sitting:0b,PortalCooldown:0,SkinID:0,IsSwimming:0b,MarkVariant:0,InventoryVersion:\"1.14.20\",Ticking:0b,IsAngry:0b,Motion:[0.0f,0.0f,0.0f],IsAutonomous:0b,OnGround:1b,Color2:0b,Rotation:[-1.5359192f,0.0f],LastOutputParams:[\"gamemode c\",\"[\",\"m=adventur\"],Command:\"/gamemode c @p\",OwnerNew:-1l,Fire:0s,Sheared:0b,LastOutput:\"commands.generic.syntax\",IsGlobal:0b,Strength:0,TrackOutput:1b}}}";
	//std::unique_ptr<Tag> ptr = Mojangson::parseTag(h);
	//logF("string: %s", h.c_str());

	std::stringstream build;
	tag->write(build);
	//C_ItemStack* stack = g_Data.getLocalPlayer()->getSelectedItem();
	//Fuck* y = reinterpret_cast<Fuck*>(stack);
	//y->tag->write(build);
	//logF("item %llX", stack);
	g_Data.getClientInstance()->getGuiData()->displayClientMessage(&build.str());

	return true;
}
