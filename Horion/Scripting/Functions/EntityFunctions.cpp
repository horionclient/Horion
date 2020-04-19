#include "EntityFunctions.h"

// this fixes intellisense somehow
#ifndef ENTITY_INVALID
#define ENTITY_INVALID \
	THROW(L"Entity is invalid! Check if your entity is still valid with entity.isValid()")
#endif

JsValueRef EntityFunctions::prototype;

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
	
	return scriptMgr.prepareVector3(*ent->getPos());
}

JsValueRef CALLBACK EntityFunctions::getVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(ent->velocity);
}

JsValueRef CALLBACK EntityFunctions::isOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return chakra.toBoolean(ent->onGround);
}

JsValueRef CALLBACK EntityFunctions::getSize(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(vec3_t(ent->width, ent->height, ent->width));
}

JsValueRef CALLBACK EntityFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		wchar_t* name = L"Entity(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}

	wchar_t* name = L"Entity(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}

JsValueRef CALLBACK EntityFunctions::getViewAngles(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	return scriptMgr.prepareVector3(vec3_t(ent->viewAngles, 0));
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