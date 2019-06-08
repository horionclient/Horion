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

	if (this->delay >= 30) {
		for (int px = x - 8; px < x + 8; px++)
		{
			for (int py = y - 8; py < y + 8; py++)
			{
				for (int pz = z - 8; pz < z + 8; pz++)
				{
					vec3_ti* bedPos = new vec3_ti(px, py, pz);

					if ((*(g_Data.getLocalPlayer()->region->getBlock(vec3_ti(*bedPos))->blockLegacy))->blockId == 26) {

						//g_Data.getCGameMode()->destroyBlock(bedPos, 0);
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
