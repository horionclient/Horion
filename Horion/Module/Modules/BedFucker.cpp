#include "BedFucker.h"



BedFucker::BedFucker() : IModule(VK_NUMPAD9, Category::EXPLOITS, "Destroys beds around you")
{
	registerIntSetting("Range", &this->range, this->range, 1, 10);
	registerBoolSetting("Beds", &this->beds, this->beds);
	registerBoolSetting("Eggs", &this->eggs, this->eggs);
	registerBoolSetting("Cakes", &this->cakes, this->cakes);
	registerBoolSetting("Treasures", &this->treasures, this->treasures);
}


BedFucker::~BedFucker()
{
}

const char* BedFucker::getModuleName()
{
	return ("Fucker");
}

void BedFucker::onEnable(){

	this->delay = 0;

}

void BedFucker::onTick(C_GameMode* gm) {

	this->delay++;
	int x = (int) g_Data.getLocalPlayer()->aabb.lower.x;
	int y = (int) g_Data.getLocalPlayer()->aabb.lower.y;
	int z = (int) g_Data.getLocalPlayer()->aabb.lower.z;

	if (this->delay >= 5) {
		this->delay = 0;
		for (int px = x - range; px < x + range; px++)
		{
			for (int py = y - range; py < y + range; py++)
			{
				for (int pz = z - range; pz < z + range; pz++)
				{
					vec3_ti* bedPos = new vec3_ti(px, py, pz);

					bool destroy = false;

					if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 26 && this->beds) destroy = true; // Beds
					if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 122 && this->eggs) destroy = true; // Dragon Eggs
					if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 92 && this->cakes)  destroy = true; // Cakes
					{
						g_Data.getCGameMode()->destroyBlock(bedPos, 0);
						g_Data.getLocalPlayer()->swingArm();
						delete bedPos;
						return;

					}
					delete bedPos;
				}

			}
		}
		if (this->treasures) {
			g_Data.forEachEntity([](C_Entity* ent, bool b) {
				std::string name = ent->getNameTag()->getText();
				int id = ent->getEntityTypeId();
				if (name.find("Treasure") != std::string::npos && id == 319) {
					g_Data.getCGameMode()->attack(ent);
					g_Data.getLocalPlayer()->swingArm();
				}
				});
		}
	}
	
}

void BedFucker::onDisable() {

	this->delay = 0;

}
