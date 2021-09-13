#include "InfiniteAura.h"

InfiniteAura::InfiniteAura() : IModule(0, Category::COMBAT, "Killaura with infinite reach") {
	registerBoolSetting("multiaura", &this->isMulti, this->isMulti);
	registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	registerFloatSetting("range", &this->range, this->range, 15, 100);
	registerIntSetting("delay", &this->delay, this->delay, 15, 20);
}

InfiniteAura::~InfiniteAura() {
}

const char* InfiniteAura::getModuleName() {
	return ("InfiniteAura");
}

static std::vector<C_Entity*> targetList0;

void findEntities(C_Entity* currentEntity, bool isRegularEntitie) {
	static auto infiniteReachMod = moduleMgr->getModule<InfiniteReach>();

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (currentEntity == 0)
		return;

	if (currentEntity->timeSinceDeath > 0 || currentEntity->damageTime >= 7)
		return;

	if (FriendList::findPlayer(currentEntity->getNameTag()->getText()))  // Skip Friend
		return;
	
	if (infiniteReachMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
			return;
		
		if (currentEntity->getEntityTypeId() == 64)  // item
			return;
	} else {
		if (!Target::isValidTarget(currentEntity))
			return;
	}

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < infiniteReachMod->range) {
			targetList0.push_back(currentEntity);
		}
}

void InfiniteAura::onTick(C_GameMode* gm) {

	//Loop through all our players and retrieve their information
	targetList0.clear();

	g_Data.forEachEntity(findEntities);
	Odelay++;

	if (targetList0.size() > 0 && Odelay >= delay) {
		g_Data.getLocalPlayer()->swingArm();

		float calcYaw = (gm->player->yaw + 90) * (PI / 180);
		float calcPitch = (gm->player->pitch) * -(PI / 180);

		float teleportX = cos(calcYaw) * cos(calcPitch) * 3.5f;
		float teleportZ = sin(calcYaw) * cos(calcPitch) * 3.5f;
		C_MovePlayerPacket teleportPacket;

		if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "mco.cubecraft.net") == 0) {
			vec3_t pos = *g_Data.getLocalPlayer()->getPos();

			C_MovePlayerPacket movePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);

			pos.y += 0.35f;

			movePlayerPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), pos);
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&movePlayerPacket);
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (int i = 0; i < targetList0.size(); i++) {
				vec3_t pos = *targetList0[i]->getPos();
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
				g_Data.getCGameMode()->attack(targetList0[i]);
				teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			}
		} else {
			vec3_t pos = *targetList0[0]->getPos();
			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), vec3_t(pos.x - teleportX, pos.y, pos.z - teleportZ));
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
			g_Data.getCGameMode()->attack(targetList0[0]);
			teleportPacket = C_MovePlayerPacket(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&teleportPacket);
		}
		Odelay = 0;
	}
}
