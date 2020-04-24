#include "Vector3Functions.h"

std::optional<vec3_t> Vector3Functions::getVecFromValue(JsValueRef ref) {
	JsValueType type;
	auto err = chakra.JsGetValueType_(ref, &type);
	if (type != JsObject || err != JsNoError)
		return std::optional<vec3_t>();

	bool hasExternalData = false;
	err = chakra.JsHasExternalData_(ref, &hasExternalData);
	if (!hasExternalData || err != JsNoError)
		return std::optional<vec3_t>();

	JVector3* vecInfo;
	err = chakra.JsGetExternalData_(ref, reinterpret_cast<void**>(&vecInfo));
	if (vecInfo->dataType != Vector3DataType || err != JsNoError)
		return std::optional<vec3_t>();
	return std::optional<vec3_t>(vecInfo->vec);
}

std::optional<vec3_t> Vector3Functions::getVecFromArguments(JsValueRef* args, int argCount) {
	if (argCount <= 0)
		return std::optional<vec3_t>();

	auto vec = Vector3Functions::getVecFromValue(args[0]);

	if (vec.has_value())
		return vec.value();

	if (argCount < 3)
		return std::optional<vec3_t>();

	bool isValid = true;
	JsValueType type;
	for (int i = 0; i < 3; i++) {
		auto err = chakra.JsGetValueType_(args[i], &type);
		if (err != JsNoError || type != JsNumber) {
			isValid = false;
			break;
		}
	}

	if (!isValid)
		return std::optional<vec3_t>();

	double x, y, z;
	int err = 0;
	err |= (int)chakra.JsNumberToDouble_(args[0], &x);
	err |= (int)chakra.JsNumberToDouble_(args[1], &y);
	err |= (int)chakra.JsNumberToDouble_(args[2], &z);
	if ((JsErrorCode)err != JsNoError)
		return std::optional<vec3_t>();

	return std::optional<vec3_t>(vec3_t(x, y, z));
}

JsValueRef CALLBACK Vector3Functions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	JsValueRef isValidBoolean;
	chakra.JsBoolToBoolean_(vecOpt.has_value(), &isValidBoolean);

	return isValidBoolean;
}

JsValueRef CALLBACK Vector3Functions::getX(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toNumber(vecOpt->x);
}

JsValueRef CALLBACK Vector3Functions::getY(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toNumber(vecOpt->y);
}

JsValueRef CALLBACK Vector3Functions::getZ(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toNumber(vecOpt->z);
}

JsValueRef CALLBACK Vector3Functions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}
	auto vec = vecOpt.value();

	wchar_t name[80];
	int length = swprintf_s(name, L"Vec3(x=%.2f, y=%.2f, z=%.2f)", vec.x, vec.y, vec.z);
	JsValueRef ref;
	chakra.JsPointerToString_(name, length, &ref);
	return ref;
}

JsValueRef CALLBACK Vector3Functions::constructor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	if (argumentCount != 4) {
		chakra.throwTypeException(L"Vec3 constructor needs 3 parameters!");
		return JS_INVALID_REFERENCE;
	}

	double x, y, z;
	chakra.JsNumberToDouble_(arguments[1], &x);
	chakra.JsNumberToDouble_(arguments[2], &y);
	chakra.JsNumberToDouble_(arguments[3], &z);

	return scriptMgr.prepareVector3(vec3_t((float)x, (float)y, (float)z), reinterpret_cast<ContextObjects*>(callbackState));
}