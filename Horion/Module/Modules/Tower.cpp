#include "Tower.h"

#include "../../DrawUtils.h"

Tower::Tower() : IModule(0, Category::WORLD, "Like scaffold, but vertically and a lot faster") {
	registerFloatSetting("motion", &this->motion, this->motion, 0.3f, 1.f);
}

Tower::~Tower() {
}

const char* Tower::getModuleName() {
	return ("Tower");
}

bool Tower::tryTower(vec3_t blockBelow) {
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();

	if (input == nullptr)
		return false;

	blockBelow = blockBelow.floor();

	DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 0.4f);

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate && GameData::isKeyDown(*input->spaceBarKey)) {
			vec3_t moveVec;
			moveVec.x = g_Data.getLocalPlayer()->velocity.x;
			moveVec.y = motion;
			moveVec.z = g_Data.getLocalPlayer()->velocity.z;
			g_Data.getLocalPlayer()->lerpMotion(moveVec);
			g_Data.getCGameMode()->buildBlock(&blok, i);

			return true;
		}
	}
	return false;
}

void Tower::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if (!selectedItem->isValid() || !(*selectedItem->item)->isBlock())  // Block in hand?
		return;

	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
	blockBelow.y -= g_Data.getLocalPlayer()->height;
	blockBelow.y -= 0.5f;

	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexy();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel.normalize();  // Only use values from 0 - 1

	if (!tryTower(blockBelow)) {
		if (speed > 0.05f) {
			blockBelow.z -= vel.z * 0.4f;
			if (!tryTower(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryTower(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryTower(blockBelow);
				}
			}
		}
	}
}
