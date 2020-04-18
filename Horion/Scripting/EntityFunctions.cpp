#include "EntityFunctions.h"

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