#include "ChestESP.h"



ChestESP::ChestESP() : IModule('H')
{
}


ChestESP::~ChestESP()
{
}

std::string ChestESP::getModuleName()
{
	return std::string("ChestESP");
}

void ChestESP::onPostRender() {


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


	//for (size_t i = 0; i < listSize; i++) {
	C_ChestBlockActor* Chest = g_Data.getCChestBlock();
	if (Chest != 0) {
		DrawUtils::setColor(0.4f, 0.4f, 0.4f, 0.2f);
		//DrawUtils::drawEntityBox(current, max(0.2f, 1 / max(1, g_Data.getLocalPlayer()->eyePos0.dist(current->eyePos0)))); // Fancy math to give an illusion of good esp
	}

}

