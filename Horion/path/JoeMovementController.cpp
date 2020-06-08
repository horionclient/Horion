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

	auto walkTarget = end;
	bool enableNextSegmentSmoothing = true;
	float dComp = 3;

	// we should probably make seperate classes for each segment type at some point, but im just doing it here for now for faster prototyping
	switch(curSeg.getSegmentType()){
	case JUMP: {
		if(player->onGround){
			if(fabsf(pPos.y - start.y) < 0.1f){
				movementHandler->isJumping = 1;
			}else if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->currentPathSegment++;
				break;
			}
		}
		goto WALK;
	} break;
	case DROP: {
		if(player->onGround){
			if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->currentPathSegment++;
				break;
			}
		}
		goto WALK;
	} break;
	case PARKOUR_JUMP_SINGLE: {
		if(player->onGround){
			if(fabsf(pPos.y - end.y) < 0.1f && pPos.dist(end) < 0.5f){// Check for end condition
				this->currentPathSegment++;
				break;
			}
			auto tangent = end.sub(start);
			tangent.y = 0;
			tangent = tangent.normalize();

			float distToEnd = end.sub(pPos).dot(tangent);
			if(distToEnd > 0 && distToEnd < 0.5f + 0.35f){
				// maybe stuck on the block above end pos?
				walkTarget = end;
				goto WALK;
			}

			auto lastPossibleJumpTarget = start.add(tangent.mul(0.5f + 0.3f));
			walkTarget = start.add(tangent.mul(0.6f));

			auto posToJumpTarg = lastPossibleJumpTarget.sub(pPos).dot(tangent);
			if(posToJumpTarg < 0.3f && posToJumpTarg > 0 && player->velocity.dot(tangent) > 0.07f){
				// jump
				movementHandler->isJumping = 1;
				goto WALK;
			}
			goto WALK;
		}else{
			enableNextSegmentSmoothing = false;
			walkTarget = end;
			dComp = 10;
			goto WALK;
		}
	} break;
	case WATER_WALK: {
		if(player->isInWater()){
			movementHandler->isJumping = 1;
			if(pPos.sub(end).magnitudexz() < 0.2f){
				this->currentPathSegment++;
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
			this->currentPathSegment++;
			break;
		}

	} break;
	default:
		__debugbreak();
	}
}
int JoeMovementController::getCurrentPathSegment() const {
	return currentPathSegment;
}
