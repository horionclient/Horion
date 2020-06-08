#include "CChestBlockActor.h"

#include "../Utils/Utils.h"

bool C_ChestBlockActor::isBarrelBlock() {
	GamerTextHolder alloc;
	Utils::CallVFunc<25, void, GamerTextHolder*, __int64>(this, &alloc, 0);
	return strcmp(alloc.getText(), "container.barrel") == 0;
}
bool C_ChestBlockActor::isShulkerBlock() {
	GamerTextHolder alloc;
	Utils::CallVFunc<25, void, GamerTextHolder*, __int64>(this, &alloc, 0);
	return strcmp(alloc.getText(), "container.shulker") == 0;
}
AABB C_ChestBlockActor::getObstructionAABB() {
	void* coolPtr = malloc(sizeof(AABB) + 4);
	Utils::CallVFunc<40, void, void*>(this, coolPtr);
	AABB ret = *reinterpret_cast<AABB*>(coolPtr);
	free(coolPtr);
	return ret;
}
