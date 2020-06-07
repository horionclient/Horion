#include "JoeMovementController.h"

#include "../../Utils/Logger.h"
#include <utility>
JoeMovementController::JoeMovementController(JoePath path) : currentPath(std::move(path)) {
}


void JoeMovementController::step(C_LocalPlayer *player, C_MoveInputHandler *movementHandler) {
	movementHandler->clearMovementState();
	if(this->currentPathSegment < 0 || this->currentPathSegment >= this->currentPath.getNumSegments()){
		this->overrideViewAngles = false;

		return;
	}

	auto pPos = player->eyePos0;
	pPos.y -= 1.62f;

	auto curSeg = this->currentPath.getSegment(this->currentPathSegment);
	auto startBpos = curSeg.getStart();
	auto start = startBpos.toVec3t().add(0.5f, 0, 0.5f);
	auto endBpos = curSeg.getEnd();
	auto end = endBpos.toVec3t().add(0.5f, 0, 0.5f);

	// we should probably make seperate classes for each segment type at some point, but im just doing it here for now for faster prototyping
	switch(curSeg.getSegmentType()){
	case JUMP:
		if(fabsf(pPos.y - start.y) < 0.1f && player->onGround){
			movementHandler->isJumping = 1;
		}
	case DROP:
	case WALK: {

		/*vec2_t angle = pPos.CalcAngle(end);
		vec2_t appl = angle.sub(player->viewAngles).normAngles();
		appl.x = 0;

		player->applyTurnDelta(&appl);*/

		vec3_t diff3d = end.sub(pPos);
		vec2_t diff2d = {diff3d.x, diff3d.z};
		diff2d = diff2d.div(fminf(0.1f, diff2d.magnitude()));
		float yaw = player->yaw;
		auto forward = vec2_t::fromAngle(yaw * RAD_DEG);
		auto right = forward.cross();

		movementHandler->forwardMovement = forward.dot(diff2d);
		movementHandler->sideMovement = -right.dot(diff2d);

		if(pPos.dist(end) < 0.2f){
			this->currentPathSegment++;
		}

	} break;
	default:
		__debugbreak();
	}


}
