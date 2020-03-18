#include "Nbt.h"

Nbt::Nbt() : IModule(0x0, Category::PLAYER, "Print NBT Tags, Left click on mobs") {
}

Nbt::~Nbt() {
}

const char* Nbt::getModuleName() {
	return ("Nbt");
}

void Nbt::onTick(C_GameMode* gm) {
	if (!GameData::canUseMoveKeys()) {
		return;
	}
	PointingStruct* pointingStruct = g_Data.getClientInstance()->getPointerStruct();
	
	if (GameData::isRightClickDown()) {  // && Utils::getClipboardText() != this->lastCopy) {
		if (pointingStruct->entityPtr != nullptr) {
			std::unique_ptr<CompoundTag> tag = std::make_unique<CompoundTag>();
			pointingStruct->entityPtr->save(tag.get());
			std::stringstream build;
			tag->write(build);
			if (this->lastCopy == build.str())
				return;
			this->lastCopy = build.str();
			Utils::setClipboardText(this->lastCopy);
			g_Data.getGuiData()->displayClientMessageF("%s%s", GREEN, "CompoundTag Copied :");
			g_Data.getClientInstance()->getGuiData()->displayClientMessage(&build.str());
		}
	}
}
