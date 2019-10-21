#include "Glide.h"



Glide::Glide() : IModule('U', Category::MOVEMENT, "Float down")
{
	this->registerFloatSetting("value", &this->glideMod, this->glideMod, -2, 1);
}


Glide::~Glide()
{
}

const char* Glide::getModuleName()
{
	if (isEnabled()) {
		static char modName[30]; // This is kinda ghetto rn, there should be a better way to make this...
		sprintf_s(modName, 30, "Glide [%.2f]", glideModEffective);
		return modName;
	}
	else
		return ("Glide");
	
}

void Glide::onTick(C_GameMode* gm) {
	if (gm->player != nullptr) {
		glideModEffective = glideMod;
		if (GameData::isKeyDown(VK_SPACE))
			glideModEffective += 0.2f;
		if (GameData::isKeyDown(VK_SHIFT))
			glideModEffective -= 0.2f;
		gm->player->velocity.y = glideModEffective;
	}
		
}

const char * Glide::getRawModuleName()
{
	return "Glide";
}
