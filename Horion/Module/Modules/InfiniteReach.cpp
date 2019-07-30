#include "InfiniteReach.h"

InfiniteReach::InfiniteReach() : IModule('P', COMBAT) // <-- keybind
{
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 15, 100);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
}


InfiniteReach::~InfiniteReach()
{
}

const char* InfiniteReach::getModuleName()
{
	return ("InfiniteReach");
}

static std::vector <C_Entity*> targetList0;

void findEntities(C_Entity* currentEntity, bool isRegularEntitie) {
	static InfiniteReach* InfiniteReachMod = static_cast<InfiniteReach*>(moduleMgr->getModule<InfiniteReach>());
	if (InfiniteReachMod == 0)
		InfiniteReachMod = static_cast<InfiniteReach*>(moduleMgr->getModule<InfiniteReach>());
	else {
		if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
			return;

		if (currentEntity == 0)
			return;

		if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
			return;

		if (FriendList::findPlayer(currentEntity->getNameTag()->getText())) // Skip Friend
			return;

		if (!Target::isValidTarget(currentEntity))
			return;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < InfiniteReachMod->range && targetList0.size() < 5)  
		{
			targetList0.push_back(currentEntity);
		}
	}

}

void InfiniteReach::onTick(C_GameMode* gm)
{
	if (!g_Data.isInGame())
		return;

	//Loop through all our players and retrieve their information
	targetList0.clear();

	g_Data.forEachEntity(findEntities);
	Odelay++;
	if (targetList0.size() > 0 && Odelay >= delay)
	{
		// Attack all entitys in targetList 
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) {

				g_Data.getLocalPlayer()->swingArm();
				C_MovePlayerPacket* a = new C_MovePlayerPacket(g_Data.getLocalPlayer(),*targetList0[i]->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
				g_Data.getCGameMode()->attack(targetList0[i]);
				delete a;
				a = new C_MovePlayerPacket(g_Data.getLocalPlayer(),*g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
				delete a;
				
			}

		}
		else{
			g_Data.getLocalPlayer()->swingArm();
			C_MovePlayerPacket* a = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *targetList0[0]->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
			g_Data.getCGameMode()->attack(targetList0[0]);
			delete a;
			a = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a);
			delete a;
			
		}
		Odelay = 0;
	}
}


