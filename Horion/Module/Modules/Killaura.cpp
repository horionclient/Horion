#include "Killaura.h"

Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	this->registerBoolSetting("MultiAura", &this->isMulti, this->isMulti);
	this->registerBoolSetting("MobAura", &this->isMobAura, this->isMobAura);
	this->registerFloatSetting("range", &this->range, this->range, 2.f, 20.f);
	this->registerIntSetting("delay", &this->delay, this->delay, 0, 20);
	this->registerBoolSetting("hurttime", &this->hurttime, this->hurttime);
	this->registerBoolSetting("AutoWeapon", &this->autoweapon, this->autoweapon);
	this->registerBoolSetting("Silent Rotations", &this->silent, this->silent);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<C_Entity*> targetList;

void findEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (killauraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
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

void Killaura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
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

	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(findEntity);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {

		if (autoweapon) findWeapon();

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					g_Data.getCGameMode()->attack(i);
				}
			}
		}
		else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				g_Data.getCGameMode()->attack(targetList[0]);
			}
		}
		Odelay = 0;
	}
}

void Killaura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		this->setEnabled(false);
}

void Killaura::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}

void Killaura::onPostRender(C_MinecraftUIRenderContext* ctx) {
	if (!targetList.empty() && targetHUD && g_Data.isInGame()) {
		vec4_t tempPos = vec4_t(70.f, 70.f, 50.f, 40.f); //temp pos for the text pos, so we can create a pos that doesn't have player names overlapping from the box to the screen
		vec2_t textPos = vec2_t(tempPos.y, tempPos.x); //text pos
		vec4_t pos = vec4_t(70.f, 70.f, 100.f + textPos.x, 90.f); //pos for using
		static float rcolors2[4];          // Rainbow color array RGBA
		static float disabledRcolors2[4];  // Rainbow Colors, but for disabled modules
		static float currColor[4];         // ArrayList colors

		// Rainbow color updates
		{
			Utils::ApplyRainbow(rcolors2, 0.001f);  // Increase Hue of rainbow color array
			disabledRcolors2[0] = std::min(1.f, rcolors2[0] * 0.4f + 0.2f);
			disabledRcolors2[1] = std::min(1.f, rcolors2[1] * 0.4f + 0.2f);
			disabledRcolors2[2] = std::min(1.f, rcolors2[2] * 0.4f + 0.2f);
			disabledRcolors2[3] = 1;
		}
		currColor[3] = rcolors2[3];
		Utils::ColorConvertRGBtoHSV(rcolors2[0], rcolors2[1], rcolors2[2], currColor[0], currColor[2], currColor[2]);
		Utils::ColorConvertHSVtoRGB(currColor[0], currColor[2], currColor[3], currColor[0], currColor[0], currColor[1]);
		std::string name = targetList[0]->getNameTag()->getText();
		std::string distance = "Distance: " + std::to_string((*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos()));

		DrawUtils::drawRectangle(pos, currColor, 1.f);
		DrawUtils::fillRectangle(pos, MC_Color(0, 0, 0), 1.f);
		DrawUtils::drawText(textPos, &name, currColor, 1.f);
		textPos.y += 10.f;
		DrawUtils::drawText(textPos, &distance, currColor, 1.f);
	}
}
