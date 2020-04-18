#include "EntityFunctions.h"

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
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}
	
	return scriptMgr.prepareVector3(ent->aabb.lower);
}

JsValueRef CALLBACK EntityFunctions::getVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}

	return scriptMgr.prepareVector3(ent->velocity);
}

JsValueRef CALLBACK EntityFunctions::isOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toBoolean(ent->onGround);
}

JsValueRef CALLBACK EntityFunctions::getSize(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}

	return scriptMgr.prepareVector3(vec3_t(ent->width, ent->height, ent->width));
}