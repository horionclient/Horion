#include "ChestESP.h"



ChestESP::ChestESP() : IModule('H', Category::VISUAL, "ESP for chests")
{
}


ChestESP::~ChestESP()
{
}

const char* ChestESP::getModuleName()
{
	return ("ChestESP");
}

void ChestESP::onPostRender() {

	if (!g_Data.isInGame() || !GameData::canUseMoveKeys())
		return;
	//if (listSize < 1000 && listSize > 1) {
	static float rcolors[4];
	if (rcolors[3] < 1) {
		rcolors[0] = 0.2f;
		rcolors[1] = 0.2f;
		rcolors[2] = 1.f;
		rcolors[3] = 1;
	}

	Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]); // premium code right here

	rcolors[0] += 0.0015f;
	if (rcolors[0] >= 1)
		rcolors[0] = 0;

	Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);


	std::set<std::shared_ptr<AABB>>* chestList = g_Data.getChestList();

	for (auto iter = chestList->begin(); iter != chestList->end(); ++iter){
		DrawUtils::setColor(1.f, 0.3f, 0.3f, 0.2f);
		DrawUtils::drawBox((*iter)->lower, (*iter)->upper, max(0.2f, 1 / max(1, g_Data.getLocalPlayer()->eyePos0.dist((*iter)->lower)))); // Fancy math to give an illusion of good esp
	}

}

void ChestESP::onTick(C_GameMode * gm)
{
	tickTimeout++;
	if (tickTimeout > 60) {
		tickTimeout = 0;
		g_Data.getChestList()->clear();
	}
}

