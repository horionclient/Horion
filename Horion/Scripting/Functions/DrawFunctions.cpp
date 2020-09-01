#include "DrawFunctions.h"
#include "../../DrawUtils.h"

JsValueRef CALLBACK DrawFunctions::drawLine3d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;
	auto startOpt = Vector3Functions::getVecFromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}
	// Note that &arguments[argIndex] might point to invalid memory here, but the function should abort because argumentCount - argIndex equals 0
	auto endOpt = Vector3Functions::getVecFromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!endOpt.has_value()) {
		THROW(L"Invalid end vector!");
	}
	DrawUtils::drawLine3d(*startOpt, *endOpt);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::drawLinestrip3d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	if (argumentCount < 2){
		THROW(L"Not enough arguments supplied");
	}

	auto arr = arguments[1];
	JsValueType type = JsUndefined;
	chakra.JsGetValueType_(arr, &type);
	if(type != JsArray){
		THROW(L"Array required as first argument");
	}

	JsValueRef lengthProp;
	chakra.getProperty(arr, L"length", &lengthProp);
	int arrayLength = -1;
	chakra.JsNumberToInt_(lengthProp, &arrayLength);

	if(arrayLength < 2){
		THROW(L"Array needs at least 2 points");
	}
	if(arrayLength > 100000){
		THROW(L"Array too big");
	}

	std::vector<vec3_t> pointList;
	pointList.reserve(arrayLength);
	for(int i = 0; i < arrayLength; i++){
		auto val = chakra.arrayGet(arr, i);
		auto valueOpt = Vector3Functions::getVecFromValue(val);
		if(!valueOpt.has_value()){
			THROW(L"Points in array need to be of type \"Vec3\"");
		}

		pointList.push_back(*valueOpt);
	}

	DrawUtils::drawLinestrip3d(pointList);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::setColor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	if (argumentCount < 4){
		THROW(L"Invalid arguments (3 floats needed, 4th optional)!");
	}

	bool hasAlpha = argumentCount >= 5;

	bool isValid = true;
	JsValueType type;
	for (int i = 0; i < (hasAlpha ? 4 : 3); i++) {
		auto err = chakra.JsGetValueType_(arguments[i + 1], &type);
		if (err != JsNoError || type != JsNumber) {
			isValid = false;
			break;
		}
	}

	if (!isValid){
		THROW(L"Invalid arguments (3 doubles needed, 4th optional)!");
	}

	double r, g, b, a = 1;
	int err = 0;
	err |= (int)chakra.JsNumberToDouble_(arguments[1], &r);
	err |= (int)chakra.JsNumberToDouble_(arguments[2], &g);
	err |= (int)chakra.JsNumberToDouble_(arguments[3], &b);
	if(hasAlpha)
		err |= (int)chakra.JsNumberToDouble_(arguments[4], &a);
	if ((JsErrorCode)err != JsNoError){
		THROW(L"Invalid arguments (can't convert to double)");
	}

	DrawUtils::setColor((float)r, (float)g, (float)b, (float)a);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::drawLine2d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;
	auto startOpt = Vector3Functions::getVecFromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}
	if(startOpt->z != 0){
		THROW(L"Z component must be 0");
	}
	// Note that &arguments[argIndex] might point to invalid memory here, but the function should abort because argumentCount - argIndex equals 0
	auto endOpt = Vector3Functions::getVecFromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!endOpt.has_value()) {
		THROW(L"Invalid end vector!");
	}
	DrawUtils::drawLine3d(*startOpt, *endOpt);

	return chakra.trueValue();
}