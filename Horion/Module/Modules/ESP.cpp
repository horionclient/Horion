#include "ESP.h"



ESP::ESP() : IModule('O', Category::VISUAL, "Makes it easier to find entities around you")
{
	this->registerBoolSetting("rainbow", &this->doRainbow, this->doRainbow);
	this->registerBoolSetting("MobEsp", &this->isMobEsp, this->isMobEsp);
}


ESP::~ESP()
{
}

const char* ESP::getModuleName()
{
	return ("ESP");
}

static float rcolors[4];

void doRenderStuff(C_Entity* ent,bool isRegularEntitie) {
	static ESP* espMod = reinterpret_cast<ESP*>(moduleMgr->getModule<ESP>());
	if(espMod == nullptr)
		espMod = reinterpret_cast<ESP*>(moduleMgr->getModule<ESP>());
	else {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		if (ent != localPlayer) {
			
			if (Target::isValidTarget(ent)) {
				
				if(espMod->doRainbow)
					DrawUtils::setColor(rcolors[0], rcolors[1], rcolors[2], max(0.1f, min(1.f, 15 / (ent->damageTime + 1))));
				else
					DrawUtils::setColor(0.9f, 0.9f, 0.9f, max(0.1f, min(1.f, 15 / (ent->damageTime + 1))));
			}
			else if (espMod->isMobEsp && !isRegularEntitie)
			{
				if (ent->getNameTag()->getTextLength() <= 1 && ent->getEntityTypeId() == 63)
					return;

				if (ent->isInvisible())
					return;

				if (!g_Data.getLocalPlayer()->canAttack(ent, false))
					return;
				DrawUtils::setColor(0.2f, 0.2f, 0.9f, max(0.1f, min(1.f, 15 / (ent->damageTime + 1))));
			}
			else
				DrawUtils::setColor(0.4f, 0.4f, 0.4f, 0.2f);

			DrawUtils::drawEntityBox(ent, max(0.2f, 1 / max(1, (*localPlayer->getPos()).dist(*ent->getPos())))); // Fancy math to give an illusion of good esp
		}
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