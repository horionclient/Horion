#include "Glide.h"



Glide::Glide() : IModule('U', MOVEMENT)
{
}


Glide::~Glide()
{
}

std::string Glide::getModuleName()
{
	if (isEnabled()) {
		char yeet[30]; // This is kinda ghetto rn, there should be a better way to make this...
		sprintf_s(yeet, 30, "Glide [%.2f]", glideModEffective);
		std::string s = yeet;
		return s;
	}
	else
		return std::string("Glide");
	
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