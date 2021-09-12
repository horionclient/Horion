#include "StorageESP.h"
#include "../../DrawUtils.h"

StorageESP::StorageESP() : IModule(0, Category::VISUAL, "ESP for storage blocks") {
}

StorageESP::~StorageESP() {
}

const char* StorageESP::getModuleName() {
	return ("StorageESP");
}

void StorageESP::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (!g_Data.isInGame() || !GameData::canUseMoveKeys() || g_Data.getLocalPlayer() == nullptr)
		return;

	auto ourListLock = std::scoped_lock(this->listLock);

	for (const auto& chest : bufferedChestList) {
		auto storageID = g_Data.getLocalPlayer()->region->getBlock(chest.upper)->blockLegacy->blockId;
		float math = (float)fmax(0.3f, (float)fmin(1.f, 15));
		DrawUtils::setColor(1.f, 1.f, 1.f, math);

		vec3_t blockPos = chest.lower;
		if (blockPos.x < 0)
			blockPos.x -= 1;
		if (blockPos.z < 0)
			blockPos.z -= 1;
		storageID = g_Data.getLocalPlayer()->region->getBlock(blockPos)->toLegacy()->blockId;

		auto mathVect = vec3_t(chest.upper.floor().add(vec3_t(1.f, 1.f, 1.f)).sub(chest.upper));
		mathVect.y = floor(mathVect.y);

		if (storageID == 54) DrawUtils::setColor(1.f, 1.f, 1.f, math);                     // Normal Chest
		if (storageID == 146) DrawUtils::setColor(.92f, .14f, .14f, math);                 // Trapped Chest
		if (storageID == 130) DrawUtils::setColor(0.435294f, 0.215686f, 0.631372f, math);  // Ender Chest
		if (storageID == 458) DrawUtils::setColor(0.62, 0.31, 0.04, math);                 // Barrel
		if (storageID == 205) DrawUtils::setColor(.49f, .17f, .95f, math);                 // Undyed Shulker Box
		if (storageID == 218) DrawUtils::setColor(.08f, .91f, .99f, math);                 // Shulker Box

		DrawUtils::drawBox(chest.lower, chest.upper, (float)fmax(0.2f, 1 / (float)fmax(1, g_Data.getLocalPlayer()->eyePos0.dist(chest.lower))), true);  // Fancy math to give an illusion of good esp
	}
}

void StorageESP::onTick(C_GameMode* gm) {
	// Swap list
	auto listLock = g_Data.lockChestList();
	auto& chestList = g_Data.getChestList();
	auto ourListLock = std::scoped_lock(this->listLock);

	this->bufferedChestList.clear();
	this->bufferedChestList.insert(this->bufferedChestList.end(), chestList.begin(), chestList.end());
	chestList.clear();
}
