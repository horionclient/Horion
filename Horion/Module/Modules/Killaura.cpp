#include "Killaura.h"



Killaura::Killaura() : IModule('P', COMBAT) // <-- keybind
{
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerFloatSetting("range", &this->range, this->range, 2, 8);
}


Killaura::~Killaura()
{
}

const char* Killaura::getModuleName()
{
	return ("Killaura");
}

static std::vector <C_Entity*> targetList;

void findEntity(C_Entity* currentEntity,bool isRegularEntitie) {
	static Killaura* killauraMod = static_cast<Killaura*>(moduleMgr->getModule<Killaura>());
	if (killauraMod == 0)
		killauraMod = static_cast<Killaura*>(moduleMgr->getModule<Killaura>());
	else {
		if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
			return;

		if (currentEntity == 0)
			return;

		if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
			return;

		if (FriendList::findPlayer(currentEntity->getNameTag()->getText())) // Skip Friend
			return;

		if (killauraMod->isMobAura && !isRegularEntitie)
		{
			if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
				return;

			if (currentEntity->isInvisible() && currentEntity->getEntityTypeId() != 33) // Exception for kitmap.sylphhcf.net they use a creeper as hitbox
				return;

			if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
				return;
		}
		else 
		{
			if (!Target::isValidTarget(currentEntity))
				return;
		}
		
		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < killauraMod->range)
		{
			targetList.push_back(currentEntity);
		}
	}
	
}

void Killaura::onTick(C_GameMode* gm)
{
	if (!g_Data.isInGame())
		return;

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity);

	if (targetList.size() > 0) {
		g_Data.getLocalPlayer()->swingArm();

		// Attack all entitys in targetList 
		if (isMulti) {
			for (int i = 0; i < targetList.size(); i++)
				g_Data.getCGameMode()->attack(targetList[i]);
		}
		else
			g_Data.getCGameMode()->attack(targetList[0]);
	}
}

void Killaura::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr) 
		this->setEnabled(false);
}

