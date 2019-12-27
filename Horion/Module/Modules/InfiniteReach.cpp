#include "InfiniteReach.h"

InfiniteReach::InfiniteReach() : IModule(0x0, Category::COMBAT, "Killaura with infinite reach")
{
	this->registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	this->registerFloatSetting("range", &this->range, this->range, 15, 100);
	this->registerIntSetting("delay", &this->delay, this->delay, 15, 20);
}


InfiniteReach::~InfiniteReach()
{
}

const char* InfiniteReach::getModuleName()
{
	return ("InfiniteAura");
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
	Odelay++;
	float calcYaw = (gm->player->yaw + 90) *  (PI / 180);
	float calcPitch = (gm->player->pitch)  * -(PI / 180);

	if (targetList0.size() > 0 && Odelay >= delay)
	{
		g_Data.getLocalPlayer()->swingArm();

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket* teleportPacket = nullptr;

		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(),"mco.cubecraft.net") == 0)
		{
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();

			C_MovePlayerPacket* movePlayerPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(movePlayerPacket);
			delete movePlayerPacket;

			pos.y += 0.35f;

			movePlayerPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(movePlayerPacket);
			delete movePlayerPacket;
		}
		
		// Attack all entitys in targetList 
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) 
			{
				vec3_t pos = *targetList0[i]->getPos();
				teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
				g_Data.getCGameMode()->attack(targetList0[i]);
				delete teleportPacket;
				teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
				delete teleportPacket;
			}
		}
		else
		{
			vec3_t pos = *targetList0[0]->getPos();
			teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x-teleportX,pos.y,pos.z-teleportZ));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
			g_Data.getCGameMode()->attack(targetList0[0]);
			delete teleportPacket;
			teleportPacket = new C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(teleportPacket);
			delete teleportPacket;
		}
		Odelay = 0;
	}
}


