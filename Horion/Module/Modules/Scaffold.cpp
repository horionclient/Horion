#include "Scaffold.h"



Scaffold::Scaffold() : IModule(VK_NUMPAD1)
{
}


Scaffold::~Scaffold()
{
}

std::string Scaffold::getModuleName()
{
	return std::string("Scaffold");
}

void Scaffold::onPostRender()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;
	blockBelow.y -= g_Data.getLocalPlayer()->height + 0.3f;

	/*
	* float calcYaw = (gm->player->yaw + 90) *  (PI / 180);
	* x = cos(calcYaw);
	* z = sin(calcYaw);
	* // Maybe use advanced stuff from jetpack to enhance the chosen block
	*/

	blockBelow = blockBelow.floor();

	using yeetBoi_t = __int64(__fastcall*)(uintptr_t, const vec3_ti&);
	static yeetBoi_t yeetBoi = reinterpret_cast<yeetBoi_t>(Utils::FindSignature("40 53 48 83 EC ?? 48 8B DA 8B 52 ?? 85 D2"));
	// BlockSource::getBlock()::getMaterial()::isReplaceable()
	if ((*(uint8_t *)(*(uintptr_t *)(**(uintptr_t **)(yeetBoi(g_Data.getLocalPlayer()->region, vec3_ti(blockBelow)) + 16) + 120i64) + 7i64))) {
		DrawUtils::setColor(0.2f, 0.2f, 0.8f, 1);
		DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 0.4f);
		vec3_ti* blok = new vec3_ti(blockBelow);
		g_Data.getCGameMode()->buildBlock(blok, 0);
		delete blok;
		// Block is replaceable (Air, Water ....)
	}
	
	
}
