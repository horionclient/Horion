#include "TestCommand.h"
#include "../../../Utils/Logger.h"
#include "../../../Utils/Utils.h"
#include <sstream>

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
	C_LocalPlayer* player = g_Data.getLocalPlayer();

	struct coolBoye2 {
		__int64 start;
		__int64 end;
	};


	static coolBoye2* mItemRegistry = nullptr;

	if (mItemRegistry == nullptr) {
		uintptr_t sigOffset = FindSignature("48 8B 1D ?? ?? ?? ?? 48 3B DF 74 ?? 48 8B CB E8 ?? ?? ?? ?? 90");
		int offset = *reinterpret_cast<int*>((sigOffset + 3));                                   // Get Offset from code
		mItemRegistry = reinterpret_cast<coolBoye2*>(sigOffset + offset + /*length of instruction*/ 7);  // Offset is relative
	}

	int i = 0;
	if(mItemRegistry->start != mItemRegistry->end){
		struct itemInfo {
			char* name;
			int maxCount;
			int damage;

			itemInfo(char* name, int maxCount, int damage) : name(name), maxCount(maxCount), damage(damage) {}
		};
		std::vector<itemInfo> allItems;
		void* itemDescriptor = malloc(0x20);
		memset(itemDescriptor, 0, 0x20);
		__int64 iter = mItemRegistry->start;
		do {
			i++;
			auto ptr = *reinterpret_cast<std::shared_ptr<C_Item*>*>(iter);
			if(ptr){
				auto item = *ptr;
				auto maxStack = item->getMaxStackSize(itemDescriptor);
				if(item->isStackedByData() && item->isBlock()){
					for(int d = 0; d < 16; d++){
						allItems.emplace_back(item->tileName.getText() + 5, maxStack, d);
					}
				} else
					allItems.emplace_back(item->tileName.getText() + 5, maxStack, 0);
			}

			iter += 8;
		}while (iter != mItemRegistry->end);

		auto it = allItems.begin();

		while(it != allItems.end()){
			std::stringstream lol;
			lol << "{Items:[";
			for(int numShulkers = 0; numShulkers < 27 && it != allItems.end(); numShulkers++) {
				if (numShulkers > 0)
					lol << ",";

				lol << "{Count:1b,Slot:" << numShulkers << "b,Damage:0s,Name:\"minecraft:shulker_box\",tag:{Items:[";

				int i = 0;
				while (it != allItems.end() && i < 27) {
					if (i > 0)
						lol << ",";
					itemInfo& cur = *it;
					lol << "{Count:" << cur.maxCount << "b,Damage:" << cur.damage << "s,Name:\"" << cur.name << "\",Slot:" << i << "b}";

					i++;
					it++;
				}
				lol << "]}}";
			}
			lol << "]}";
			auto str = lol.str();
			Logger::WriteBigLogFileF(str.size() + 20, "%s", str.c_str());
		}


	}

	return true;
}
