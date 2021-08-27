#include "BowAimbot.h"

#include "../../../Utils/Target.h"
#include "../../DrawUtils.h"

std::vector<C_Entity*> targetList;

BowAimbot::BowAimbot() : IModule(0, Category::COMBAT, "Aimbot, but for bows") {
	registerBoolSetting("silent", &this->silent, this->silent);
	registerBoolSetting("predict", &this->predict, this->predict);
	registerBoolSetting("visualize", &this->visualize, this->visualize);
}

BowAimbot::~BowAimbot() {
}

struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

const char* BowAimbot::getModuleName() {
	return ("BowAimbot");
}

void findTargets(C_Entity* currentEntity, bool isRegularEntitie) {
	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 130) {
		targetList.push_back(currentEntity);
	}
}

void BowAimbot::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	targetList.clear();

	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (localPlayer->getSelectedItemId() != 300)  // Bow in hand?
		return;

	if (!(GameData::isRightClickDown() && GameData::canUseMoveKeys())) // is aiming?
		return;

	g_Data.forEachEntity(findTargets);

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
		vec3_t origin = g_Data.getLocalPlayer()->eyePos0;  // TODO: sort list
		C_Entity* entity = targetList[0];
		vec3_t pos = entity->aabb.centerPoint();
		if (predict) {
			vec3_t velocity = entity->getPos()->sub(*entity->getPosOld());
			velocity.x *= origin.dist(pos) / 2.f;
			velocity.z *= origin.dist(pos) / 2.f;
			pos = pos.add(velocity);
		}
		if(visualize)
			DrawUtils::drawBox(pos.sub(0.5), pos.add(0.5), 0.3f, true);
		pos = pos.sub(origin);
		float yaw = (atan2f(pos.z, pos.x) * DEG_RAD) - 90;
		float len = pos.magnitudexz();
		constexpr float g = 0.002f;  // nukkit = 0.012, some servers need different values
		float tmp = 1 - g * (g * (len * len) + 2 * pos.y);
		float pitch = DEG_RAD * -atanf((1 - sqrtf(tmp)) / (g * len));

		if (this->silent) {
			this->angle = vec2_t(pitch, yaw);
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			p.pitch = angle.x;
			p.yaw = angle.y;
			p.headYaw = angle.y;
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);
		} else {
			if (pitch < 89 && pitch > -89) {
				vec2_t angles = vec2_t(pitch, yaw);

				vec2_t appl = angles.sub(localPlayer->viewAngles).normAngles();
				appl.x = -appl.x;
				appl = appl.div(7);  // Smooth dat boi

				localPlayer->applyTurnDelta(&appl);
			}
		}
	}
}

void BowAimbot::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_MovePlayerPacket>() && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			movePacket->pitch = this->angle.x;
			movePacket->headYaw = this->angle.y;
			movePacket->yaw = this->angle.y;
		}
	}
}
