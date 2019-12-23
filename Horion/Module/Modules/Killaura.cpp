#include "Killaura.h"



Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerFloatSetting("range", &this->range, this->range, 2.f, 20.f);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerBoolSetting("AutoWeapon", &this->autoweapon, this->autoweapon);
}


Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
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

		if (killauraMod->isMobAura && !isRegularEntitie) {
			if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
				return;

			if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
				return;
		}
		else {
			if (!Target::isValidTarget(currentEntity))
				return;
		}
		
		float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

		if (dist < killauraMod->range) {
			targetList.push_back(currentEntity);
		}
	}
	
}

void Killaura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != NULL) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::onTick(C_GameMode* gm) {
	if (!g_Data.isInGame())
		return;

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity);

	hasTarget = !targetList.empty();

	Odelay++;
	if (hasTarget && Odelay >= delay) {
		if (autoweapon) findWeapon();

		g_Data.getLocalPlayer()->swing();

		// Attack all entitys in targetList 
		if (isMulti) {
			for (int i = 0; i < targetList.size(); i++) {
				angle = g_Data.getClientInstance()->levelRenderer->origin.CalcAngle(*targetList[i]->getPos());
				g_Data.getCGameMode()->attack(targetList[i]);
			}
		}
		else {
			angle = g_Data.getClientInstance()->levelRenderer->origin.CalcAngle(*targetList[0]->getPos());
			g_Data.getCGameMode()->attack(targetList[0]);
		}
		Odelay = 0;
	}
}

void Killaura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) 
		this->setEnabled(false);
}

void Killaura::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		vec2_t angle = this->angle;
		if (this->hasTarget) {
			C_MovePlayerPacket* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

