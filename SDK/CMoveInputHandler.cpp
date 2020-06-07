#include "CMoveInputHandler.h"
#include "../Utils/Utils.h"

__int64 C_MoveInputHandler::clearMovementState() {
	return Utils::CallVFunc<4, __int64>(this);
}
