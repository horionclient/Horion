#include "LocalPlayerFunctions.h"

#ifndef ENTITY_INVALID
#define ENTITY_INVALID                                                                                        \
	THROW(L"Entity is invalid! Check if your entity is still valid with entity.isValid()")

#endif

JsValueRef CALLBACK LocalPlayerFunctions::setPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	auto vecOpt = Vector3Functions::getVecFromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	ent->setPos(vecOpt.value());
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::setVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto vecOpt = Vector3Functions::getVecFromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	ent->velocity = vecOpt.value();
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		const wchar_t* name = L"LocalPlayer(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}
	
	const wchar_t* name = L"LocalPlayer(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}

JsValueRef CALLBACK LocalPlayerFunctions::setViewAngles(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}

	auto vecOpt = Vector3Functions::getVecFromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	vec2_t temp = vec2_t(vecOpt.value().x, vecOpt.value().y).sub(ent->viewAngles);
	reinterpret_cast<C_LocalPlayer*>(ent)->applyTurnDelta(&temp);
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::setIsOnGround(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		ENTITY_INVALID;
	}
	auto isOnGroundOptional = chakra.tryGetBoolFromArgs(&arguments[1], argumentCount - 1);

	if (!isOnGroundOptional.has_value()) {
		THROW(L"Invalid boolean!");
	}

	ent->onGround = isOnGroundOptional.value();
	return chakra.trueValue();
}
