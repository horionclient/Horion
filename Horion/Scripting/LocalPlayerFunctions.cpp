#include "LocalPlayerFunctions.h"

JsValueRef LocalPlayerFunctions::prototype;

JsValueRef CALLBACK LocalPlayerFunctions::setPosition(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}
	auto vecOpt = Vector3Functions::getVecFromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Invalid vector!");
		return JS_INVALID_REFERENCE;
	}

	ent->setPos(vecOpt.value());
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::setVelocity(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		chakra.throwTypeException(L"Entity is invalid! Check if your entity is still valid with entity.isValid()");
		return JS_INVALID_REFERENCE;
	}
	auto vecOpt = Vector3Functions::getVecFromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Invalid vector!");
		return JS_INVALID_REFERENCE;
	}

	ent->velocity = vecOpt.value();
	return chakra.trueValue();
}

JsValueRef CALLBACK LocalPlayerFunctions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = EntityFunctions::getEntityFromValue(arguments[0]);
	if (ent == nullptr) {
		wchar_t* name = L"LocalPlayer(invalid)";
		JsValueRef ref;
		chakra.JsPointerToString_(name, wcslen(name), &ref);
		return ref;
	}
	
	wchar_t* name = L"LocalPlayer(isValid=true)";
	JsValueRef ref;
	chakra.JsPointerToString_(name, wcslen(name), &ref);
	return ref;
}