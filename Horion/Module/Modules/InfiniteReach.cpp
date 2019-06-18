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

	if (targetList.size() > 0)// && delay >= 10)
	{
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec3_t h;
		h.x = 0;
		h.y = 0;
		h.z = 0;
		vec3_t myPos = *localPlayer->getPos();
		vec3_t targetPos = *targetList[0]->getPos();
		vec3_t vector;
		vector.x = targetPos.x - myPos.x;
		vector.z = targetPos.z - myPos.z;
		float a, b;

		if ((int)(vector.x) != 0) {
			a = vector.z / vector.x;
			b = localPlayer->getPos()->z - a * localPlayer->getPos()->x;
		}
	
		//Calculate myFakePos
		//int oklm = (int)(vector.);

		if ((int)vector.x == 0)
		{
			if (fakeZ == 0 && vector.z < 0)
				fakeZ = localPlayer->getPos()->z - 0.5f;
			else if (vector.z < 0)
				fakeZ -= 0.5f;
			else if (fakeX == 0 && vector.z > 0)
				fakeZ = localPlayer->getPos()->z + 0.5f;
			else if (vector.z > 0)
				fakeZ += 0.5f;
			else {
				fakeZ += 0.5f;
			}
			h.x = localPlayer->getPos()->x;
		}
		else {
			if (fakeX == 0 && vector.x < 0)
				fakeX = localPlayer->getPos()->x - 0.5f;
			else if (vector.x < 0)
				fakeX -= 0.5f;
			else if (fakeX == 0 && vector.x > 0)
				fakeX = localPlayer->getPos()->x + 0.5f;
			else if (vector.x > 0)
				fakeX += 0.5f;
			fakeZ = a * fakeX + b;
			h.x = fakeX;
		}
	
	
		
		
		h.y = targetList[0]->getPos()->y;
		h.z = fakeZ;
		vec3_t YIKES = *localPlayer->getPos();

		//Send my position 
		C_MovePlayerPacket* op1 = new C_MovePlayerPacket(localPlayer, *localPlayer->getPos());
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(op1);
		delete op1;

		//h = *targetList[0]->getPos();

		/*if (tick) {
			h.x += 2;
			h.z += 2;
			tick = false;
		}
		else {
			h.x -= 2;
			h.z -= 2;
			tick = true;
		}*/

		//Show the box of my fakePosition
		vec3_t lower;
		lower.x = h.x + 0.3f;
		lower.y = h.y - 1.62f;
		lower.z = h.z + 0.3f;
		vec3_t upper;
		upper.x = lower.x + ((*localPlayer->getAABB()).upper.x - (*localPlayer->getAABB()).lower.x);
		upper.y = lower.y + 1.8f;
		upper.z = lower.z + ((*localPlayer->getAABB()).upper.z - (*localPlayer->getAABB()).lower.z);
		DrawUtils::setColor(1.f, 0.3f, 0.3f, 0.2f);
		DrawUtils::drawBox(lower, upper, max(0.2f, 1 / max(1, h.dist(lower))));

		// Send a packet with my fakePosition
		C_MovePlayerPacket* op = new C_MovePlayerPacket(localPlayer, h);
		g_Data.getClientInstance()->loopbackPacketSender->sendToServer(op);
		delete op;

		//Attack if my fakePos is near of the target
		if ((*targetList[0]->getPos()).dist(h) < 3.0f)
		{
			localPlayer->swingArm();
			g_Data.getCGameMode()->attack(targetList[0]);
			fakeX = 0;
			fakeZ = 0;
		}
		
		delay = 0;


	}

}



void InfiniteReach::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
	fakeX = 0;
	fakeZ = 0;
	delay = 0;
}

void InfiniteReach::onDisable() {
	if (g_Data.getLocalPlayer() != nullptr) {
		fakeX = 0;
		fakeZ = 0;
	}
}
