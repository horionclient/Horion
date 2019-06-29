#include "InfiniteReach.h"

bool tick = true;


InfiniteReach::InfiniteReach() : IModule(0x0, COMBAT) // <-- keybind
{
}


InfiniteReach::~InfiniteReach()
{
}

const char* InfiniteReach::getModuleName()
{
	return ("InfiniteReach");
}

struct CompareTargetEnArray
{
	bool operator() (C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void InfiniteReach::onPostRender()
{
	delay++;
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;
	C_EntityList* entList = g_Data.getEntityList();
	if (entList == nullptr)
		return;
	size_t listSize = entList->getListSize();

	if (listSize > 10000) {
#ifdef _DEBUG
		logF("Big ent list wtf men %i", listSize);
#endif
		return;
	}


	//Loop through all our players and retrieve their information
	float maxDist = 20;
	static std::vector <C_Entity*> targetList;
	targetList.clear();
	for (size_t i = 0; i < listSize; i++)
	{
		C_Entity* currentEntity = entList->get(i);

		if (currentEntity == 0)
			break;

		if (currentEntity == localPlayer) // Skip Local player
			continue;

		if (FriendList::findPlayer(currentEntity->getNameTag()->getText())) //Skip Friend
			continue;

		if (localPlayer->getEntityTypeId() != currentEntity->getEntityTypeId()) // Skip Invalid Entity
			continue;

		if (!(currentEntity->name2.getTextLength() > 0))
			continue;

		if (currentEntity->height < 1.5f || currentEntity->width < 0.5f || currentEntity->height > 2.1f || currentEntity->width > 0.9f)
			continue;

		// i want to hit villagers ok
//		if (localPlayer->entityType2 != currentEntity->entityType2)
//			continue;

		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());;

		if (dist < maxDist)
		{
			targetList.push_back(currentEntity);
		}
	}

	if (targetList.size() > 0)
	{
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		C_MovePlayerPacket* p = new C_MovePlayerPacket(localPlayer, *targetList[0]->getPos());
		
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(p);
		localPlayer->swing();
		g_Data.getCGameMode()->attack(targetList[0]);
		delete p;
		p = new C_MovePlayerPacket(localPlayer, *localPlayer->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(p);
		
		
		delay = 0;
		delete p;
	}

}

void InfiniteReach::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
	delay = 0;
}

void InfiniteReach::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		delay = 0;
	}
}
