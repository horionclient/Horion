#include "EntityFunctions.h"
#include "../../DrawUtils.h"

// this fixes intellisense somehow
#ifndef ENTITY_INVALID
#define ENTITY_INVALID \
	THROW(L"Entity is invalid! Check if your entity is still valid with entity.isValid()")
#endif

JsValueRef CALLBACK EntityFunctions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	bool isValid = EntityFunctions::getEntityFromValue(arguments[0]) != nullptr;
	JsValueRef isValidBoolean;
	chakra.JsBoolToBoolean_(isValid, &isValidBoolean);
	
	return isValidBoolean;
}

JsValueRef CALLBACK EntityFunctions::getPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	
	return scriptMgr.prepareVector3(*ent->getPos(), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getInterpolatedPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	vec3_t* start = ent->getPosOld();
	vec3_t* end = ent->getPos();

	auto t = DrawUtils::getLerpTime();
	vec3_t lerped = start->lerp(end, t);

	return scriptMgr.prepareVector3(lerped, reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(ent->velocity, reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::isOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->onGround);
}

JsValueRef CALLBACK EntityFunctions::isInvisible(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isInvisible());
}

JsValueRef CALLBACK EntityFunctions::isInWater(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isInWater());
}

JsValueRef CALLBACK EntityFunctions::isInLava(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isInLava());
}

JsValueRef CALLBACK EntityFunctions::isSneaking(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->isSneaking());
}

JsValueRef CALLBACK EntityFunctions::getSize(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(vec3_t(ent->width, ent->height, ent->width), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		const wchar_t* name = L"Entity(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}

	const wchar_t* name = L"Entity(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}

JsValueRef CALLBACK EntityFunctions::getViewAngles(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(vec3_t(ent->viewAngles, 0), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK EntityFunctions::getPitch(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toNumber(ent->pitch);
}

JsValueRef CALLBACK EntityFunctions::getYaw(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toNumber(ent->yaw);
}