#include "ESP.h"



ESP::ESP() : IModule('O', VISUAL)
{
}


ESP::~ESP()
{
}

const char* ESP::getModuleName()
{
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(C_Entity* ent) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (ent != localPlayer) {
		if (ent->timeSinceDeath > 0)
			return;
		if (ent->name2.getTextLength() > 1 && 
			ent->height > 1.5f && ent->width > 0.5f
			&& ent->height < 2.1f && ent->width < 0.9f
			)
			DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], max(0.1f, min(1.f, 15 / (ent->damageTime + 1))));
		else
			DrawUtils::setColor(0.4f, 0.4f, 0.4f, 0.2f);

		DrawUtils::drawEntityBox(ent, max(0.2f, 1 / max(1, localPlayer->eyePos0.dist(ent->eyePos0)))); // Fancy math to give an illusion of good esp
	}
}

void ESP::onPostRender() {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {

		// Rainbow colors
		{
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
		}

		g_Data.forEachEntity(doRenderStuff);
	}
}