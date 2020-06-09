#include "JoeMovementController.h"

#include "../../Utils/Logger.h"
#include <utility>
JoeMovementController::JoeMovementController(std::shared_ptr<JoePath> path) : currentPath(path) {
}

void JoeMovementController::step(C_LocalPlayer *player, C_MoveInputHandler *movementHandler) {
	movementHandler->clearMovementState();
	if(this->stateInfo.currentPathSegment < 0 || this->stateInfo.currentPathSegment >= this->currentPath->getNumSegments()){
		this->overrideViewAngles = false;
		return;
	}

	auto pPos = player->eyePos0;
	pPos.y -= 1.62f;

	auto curSeg = this->currentPath->getSegment(this->stateInfo.currentPathSegment);

	auto startBpos = curSeg.getStart();
	auto start = startBpos.toVec3t().add(0.5f, 0, 0.5f);
	auto endBpos = curSeg.getEnd();
	auto end = endBpos.toVec3t().add(0.5f, 0, 0.5f);

	auto nextSegEnd = vec3_t();

	bool hasNextSeg = this->stateInfo.currentPathSegment < this->currentPath->getNumSegments() - 1;
	if(hasNextSeg)
		nextSegEnd = this->currentPath->getSegment(this->stateInfo.currentPathSegment + 1).getEnd().toVec3t().add(0.5f, 0, 0.5f);

	auto walkTarget = end;
	bool enableNextSegmentSmoothing = true;
	float dComp = 4;

	// we should probably make seperate classes for each segment type at some point, but im just doing it here for now for faster prototyping
	switch(curSeg.getSegmentType()){
	case JUMP: {
		if(player->onGround){
			if(fabsf(pPos.y - start.y) < 0.1f){
				movementHandler->isJumping = 1;
			}else if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->stateInfo.nextSegment();
				break;
			}
		}
		goto WALK;
	} break;
	case DROP: {
		bool inWater = player->isInWater();
		if(player->onGround || inWater){
			if(fabsf(pPos.y - end.y) < (inWater ? 0.2f : 0.1f) && pPos.sub(end).magnitudexz() < 0.5f && player->velocity.y > -0.1f){// Check for end condition
				this->stateInfo.nextSegment();
				break;
			}else if(inWater && (pPos.y < end.y || player->velocity.y < 0.12f))
				movementHandler->isJumping = 1;
		}
		goto WALK;
	} break;
	case PARKOUR_JUMP_SINGLE: {
		if(player->isSprinting())
			player->setSprinting(false);
		if(player->onGround){
			if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->stateInfo.nextSegment();
				break;
			}
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();

			float distToEnd = end.sub(pPos).dot(tangent);
			if(distToEnd > 0.f && distToEnd < 0.5f + 0.35f){
				// maybe stuck on the block above end pos?
				walkTarget = end;
				goto WALK;
			}

			auto lastPossibleJumpTarget = start.add(tangent.mul(0.5f + 0.3f));
			walkTarget = start.add(tangent); // This is not actually on a block anymore, but if we make this smaller the movement controller will stop moving at the jump target

			auto posToJumpTarg = lastPossibleJumpTarget.sub(pPos).dot(tangent);

			float maxJumpDist = 0.3f;
			if(start.sub(end).magnitudexz() <= 1.1f)
				maxJumpDist = 0.7f;
			if(posToJumpTarg < maxJumpDist && posToJumpTarg > 0 && player->velocity.dot(tangent) > 0.07f){
				// jump
				movementHandler->isJumping = 1;
				goto WALK;
			}
			goto WALK;
		}else{
			enableNextSegmentSmoothing = false;
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();
			walkTarget = end.sub(tangent.mul(0.4f));
			dComp = 5;
			goto WALK;
		}
	} break;
	case WATER_WALK: {
		if(player->isInWater()){
			movementHandler->isJumping = 1;
			if(pPos.sub(end).magnitudexz() < 0.2f){
				this->stateInfo.nextSegment();
				break;
			}
		}
		goto WALK;
	} break;
	WALK:;
	case WALK: {
		auto pPosD = pPos; // p
		pPosD.add(player->velocity.mul(dComp, 0, dComp)); // d

		vec3_t diff3d = walkTarget.sub(pPosD);
		vec2_t diff2d = {diff3d.x, diff3d.z};
		float diffMag = diff2d.magnitude();
		if(enableNextSegmentSmoothing && hasNextSeg && diffMag < 0.15f && fabsf(end.y - pPosD.y) < 0.1f){ // Start taking the next segment into account when we're very close to our destination
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

		//logF("%.2f %.2f %.2f %i", diff2d.x, diff2d.y, pPos.y, player->onGround);

		if(pPos.dist(end) < 0.2f){
			this->stateInfo.nextSegment();
			break;
		}

	} break;
	default:
		__debugbreak();
	}
}
int JoeMovementController::getCurrentPathSegment() const {
	return stateInfo.currentPathSegment;
}
