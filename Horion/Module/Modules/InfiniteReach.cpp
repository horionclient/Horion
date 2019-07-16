#include "InfiniteReach.h"

InfiniteReach::InfiniteReach() : IModule('P', COMBAT) // <-- keybind
{
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 2, 50);
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

		if (dist < InfiniteReachMod->range)
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
	delay++;
	if (targetList0.size() > 0) {
		
		vec3_t pos = *g_Data.getLocalPlayer()->getPos();
		// Attack all entitys in targetList 
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) {
		
				/*if (delay > 4) {
					C_MovePlayerPacket* a2 = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(a2);
					delete a2;
					delay = 0;
				}*/
			}
				
		}
		else if (delay > 5){
			g_Data.getLocalPlayer()->swingArm();
			vec3_t targetPos = *targetList0[0]->getPos();
			pack = new C_MovePlayerPacket(g_Data.getLocalPlayer(), targetPos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(pack);
			g_Data.getCGameMode()->attack(targetList0[0]);
			delete pack;
		/*	pack = new	C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(pack);
			delete pack;*/
			pack = nullptr;
			delay = 0;
		}
	}
}


