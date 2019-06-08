#include "BedFucker.h"



BedFucker::BedFucker() : IModule(VK_NUMPAD9, EXPLOITS)
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
	int x = g_Data.getLocalPlayer()->aabb.lower.x;
	int y = g_Data.getLocalPlayer()->aabb.lower.y;
	int z = g_Data.getLocalPlayer()->aabb.lower.z;

	if (this->delay >= 5) {
		for (int px = x - 5; px < x + 5; px++)
		{
			for (int py = y - 3; py < y + 3; py++)
			{
				for (int pz = z - 5; pz < z + 5; pz++)
				{
					vec3_ti* bedPos = new vec3_ti(px, py, pz);

					if ((*(g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->blockLegacy))->blockId == 26) {
						__debugbreak();
						g_Data.getCGameMode()->destroyBlock(bedPos, 0);
						g_Data.getLocalPlayer()->swingArm();
						this->delay = 0;
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
