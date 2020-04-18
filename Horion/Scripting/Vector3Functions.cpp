#include "Vector3Functions.h"

JsValueRef CALLBACK Vector3Functions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector3Functions::getVecFromValue(arguments[0]);
	JsValueRef isValidBoolean;
	chakra.JsBoolToBoolean_(vecOpt.has_value(), &isValidBoolean);

	return isValidBoolean;
}

std::optional<vec3_t> Vector3Functions::getVecFromValue(JsValueRef ref) {
	JsValueType type;
	chakra.JsGetValueType_(ref, &type);
	if (type != JsObject)
		return std::optional<vec3_t>();

	bool hasExternalData = false;
	chakra.JsHasExternalData_(ref, &hasExternalData);
	if (!hasExternalData)
		return std::optional<vec3_t>();

	JVector3* vecInfo;
	chakra.JsGetExternalData_(ref, reinterpret_cast<void**>(&vecInfo));
	if (vecInfo->dataType != Vector3DataType)
		return std::optional<vec3_t>();
	return std::optional<vec3_t>(vecInfo->vec);
}
