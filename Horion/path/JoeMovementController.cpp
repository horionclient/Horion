#include "JoeMovementController.h"

#include "../../Utils/Logger.h"
#include <utility>
JoeMovementController::JoeMovementController(std::shared_ptr<JoePath> path) : currentPath(path) {
}

void JoeMovementController::step(C_LocalPlayer *player, C_MoveInputHandler *movementHandler) {
	movementHandler->clearMovementState();
	if(this->currentPathSegment < 0 || this->currentPathSegment >= this->currentPath->getNumSegments()){
		this->overrideViewAngles = false;
		return;
	}

	auto pPos = player->eyePos0;
	pPos.y -= 1.62f;

	auto curSeg = this->currentPath->getSegment(this->currentPathSegment);

	auto startBpos = curSeg.getStart();
	auto start = startBpos.toVec3t().add(0.5f, 0, 0.5f);
	auto endBpos = curSeg.getEnd();
	auto end = endBpos.toVec3t().add(0.5f, 0, 0.5f);

	auto nextSegEnd = vec3_t();

	bool hasNextSeg = this->currentPathSegment < this->currentPath->getNumSegments() - 1;
	if(hasNextSeg)
		nextSegEnd = this->currentPath->getSegment(this->currentPathSegment + 1).getEnd().toVec3t().add(0.5f, 0, 0.5f);
	// we should probably make seperate classes for each segment type at some point, but im just doing it here for now for faster prototyping
	switch(curSeg.getSegmentType()){
	case JUMP:
		if(player->onGround){
			if(fabsf(pPos.y - start.y) < 0.1f){
				movementHandler->isJumping = 1;
			}else if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->currentPathSegment++;
				break;
			}
		}
	case DROP:
	case WALK: {
		auto pPosD = pPos; // p
		pPosD.add(player->velocity.mul(2.f, 0, 2.f)); // d

		vec3_t diff3d = end.sub(pPosD);
		vec2_t diff2d = {diff3d.x, diff3d.z};
		float diffMag = diff2d.magnitude();
		if(hasNextSeg && diffMag < 0.15f && fabsf(end.y - pPosD.y) < 0.1f){ // Start taking the next segment into account when we're very close to our destination
			auto tangent = nextSegEnd.sub(end).normalize();
			diff3d = end.add(tangent.mul(0.2f)).sub(pPosD);
			diff2d = {diff3d.x, diff3d.z};
			diffMag = diff2d.magnitude();
		}
		diff2d = diff2d.div(fmaxf(0.15f, diffMag));
		float yaw = player->yaw;
		auto forward = vec2_t::fromAngle(yaw * RAD_DEG);
		auto right = forward.cross();

		movementHandler->forwardMovement = forward.dot(diff2d);
		movementHandler->sideMovement = -right.dot(diff2d);

		if(pPos.dist(end) < 0.2f){
			this->currentPathSegment++;
			break;
		}

	} break;
	default:
		__debugbreak();
	}


}
