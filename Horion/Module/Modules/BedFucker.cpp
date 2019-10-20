#include "BedFucker.h"



BedFucker::BedFucker() : IModule(VK_NUMPAD9, Category::EXPLOITS, "Destroys beds around you")
{
}


BedFucker::~BedFucker()
{
}

const char* BedFucker::getModuleName()
{
	return ("BedFucker");
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
		for (int px = x - 5; px < x + 5; px++)
		{
			for (int py = y - 3; py < y + 3; py++)
			{
				for (int pz = z - 5; pz < z + 5; pz++)
				{
					vec3_ti* bedPos = new vec3_ti(px, py, pz);

					if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 26 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 122 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->toLegacy()->blockId == 92) {
						g_Data.getCGameMode()->destroyBlock(bedPos, 0);
						g_Data.getLocalPlayer()->swingArm();
						delete bedPos;
						return;
						
					}
					delete bedPos;
				}
				
			}
		}
	}
	
}

void BedFucker::onDisable() {

	this->delay = 0;

}
