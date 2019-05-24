#include "ESP.h"



ESP::ESP() : IModule('O')
{
}


ESP::~ESP()
{
}

std::string ESP::getModuleName()
{
	return std::string("ESP");
}

void ESP::onPostRender() {
	if (g_Data.getLocalPlayer() != nullptr && GameData::canUseMoveKeys()) {
		int ourent = g_Data.getLocalPlayer()->getEntityTypeId();
		C_EntityList* entList = g_Data.getEntityList();
		size_t listSize = entList->getListSize();

		if (listSize < 1000 && listSize > 1) {
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

			
			for (size_t i = 0; i < listSize; i++) {
				C_Entity* current = entList->get(i);
				if (current != g_Data.getLocalPlayer()) {
					if (current->timeSinceDeath > 0)
						continue;
					if(ourent == current->getEntityTypeId())
						DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], max(0.1f, min(1.f, 15 / (current->damageTime + 1) )));
					else
						DrawUtils::setColor(0.4f, 0.4f, 0.4f, 0.2f);
					DrawUtils::drawEntityBox(current, max(0.2f, 1 / max(1, g_Data.getLocalPlayer()->eyePos0.dist(current->eyePos0)))); // Fancy math to give an illusion of good esp
					
				}
					
			}
		}
	}
}