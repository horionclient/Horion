#include "DrawFunctions.h"
#include "../../DrawUtils.h"

JsValueRef CALLBACK DrawFunctions::drawLine3d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;
	auto startOpt = Vector3Functions::getVec3FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}
	// Note that &arguments[argIndex] might point to invalid memory here, but the function should abort because argumentCount - argIndex equals 0
	auto endOpt = Vector3Functions::getVec3FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
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
		auto valueOpt = Vector3Functions::getVec3FromValue(val);
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
	auto startOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}
	
	auto endOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!endOpt.has_value()) {
		THROW(L"Invalid end vector!");
	}
	double lineWidth = 1;
	if (argumentCount - argIndex >= 1) {
		auto status = chakra.JsNumberToDouble_(arguments[argIndex], &lineWidth);
		THROW_IF_ERROR(status, L"invalid line width");
		argIndex++;
	}
	DrawUtils::drawLine(*startOpt, *endOpt, (float)lineWidth);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::drawRectangle2d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;
	auto startOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}

	auto endOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!endOpt.has_value()) {
		THROW(L"Invalid end vector!");
	}


	double lineWidth = 1;
	if (argumentCount - argIndex >= 1) {
		auto status = chakra.JsNumberToDouble_(arguments[argIndex], &lineWidth);
		THROW_IF_ERROR(status, L"invalid line width");
		argIndex++;
	}

	vec2_t lower = {std::min(startOpt->x, endOpt->x), std::min(startOpt->y, endOpt->y)};
	vec2_t upper = {std::max(startOpt->x, endOpt->x), std::max(startOpt->y, endOpt->y)};
	DrawUtils::drawRectangle(lower, upper, (float)lineWidth);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::fillRectangle2d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;
	auto startOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!startOpt.has_value()) {
		THROW(L"Invalid start vector!");
	}

	auto endOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!endOpt.has_value()) {
		THROW(L"Invalid end vector!");
	}

	vec2_t lower = {std::min(startOpt->x, endOpt->x), std::min(startOpt->y, endOpt->y)};
	vec2_t upper = {std::max(startOpt->x, endOpt->x), std::max(startOpt->y, endOpt->y)};
	DrawUtils::fillRectangle(lower, upper);

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::drawText2d(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;

	auto textOpt = chakra.tryGetStringFromArgs(arguments[argIndex], argumentCount - argIndex);
	if (!textOpt.has_value()) {
		THROW(L"Invalid text specified");
	}
	argIndex++;

	auto posOpt = Vector2Functions::getVec2FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (!posOpt.has_value()) {
		THROW(L"Invalid pos vector!");
	}

	MC_Color color(1,1,1,1);
	double textSize = 1, alpha = 1;

	if (argumentCount - argIndex >= 1) {
		auto status = chakra.JsNumberToDouble_(arguments[argIndex], &textSize);
		THROW_IF_ERROR(status, L"invalid text size");
		argIndex++;
	}

	auto colorOpt = Vector3Functions::getVec3FromArguments(&arguments[argIndex], argumentCount - argIndex, &argIndex);
	if (colorOpt.has_value()) {
		color.r = std::clamp(colorOpt->x, 0.f, 1.f);
		color.g = std::clamp(colorOpt->y, 0.f, 1.f);
		color.b = std::clamp(colorOpt->z, 0.f, 1.f);

		if (argumentCount - argIndex >= 1) {
			auto status = chakra.JsNumberToDouble_(arguments[argIndex], &alpha);
			THROW_IF_ERROR(status, L"invalid alpha value");
			argIndex++;
		}
	}

	char* coolBeanBuffer = new char[textOpt.value().size() + 1];
	sprintf_s(coolBeanBuffer, textOpt.value().size() + 1, "%S", textOpt.value().c_str());
	std::string textStr(coolBeanBuffer);

	DrawUtils::drawText(*posOpt, &textStr, color, (float)textSize, (float)alpha);

	delete[] coolBeanBuffer;

	return chakra.trueValue();
}

JsValueRef CALLBACK DrawFunctions::getTextWidth(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;

	auto textOpt = chakra.tryGetStringFromArgs(arguments[argIndex], argumentCount - argIndex);
	if (!textOpt.has_value()) {
		THROW(L"Invalid text specified");
	}
	argIndex++;

	double textSize = 1;

	if (argumentCount - argIndex >= 1) {
		auto status = chakra.JsNumberToDouble_(arguments[argIndex], &textSize);
		THROW_IF_ERROR(status, L"invalid text size");
		argIndex++;
	}

	char* coolBeanBuffer = new char[textOpt.value().size() + 1];
	sprintf_s(coolBeanBuffer, textOpt.value().size() + 1, "%S", textOpt.value().c_str());
	std::string textStr(coolBeanBuffer);

	auto width = (double)DrawUtils::getTextWidth(&textStr, (float)textSize);

	delete[] coolBeanBuffer;

	return chakra.toNumber(width);
}

JsValueRef CALLBACK DrawFunctions::getTextLineHeight(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	int argIndex = 1;

	double textSize = 1;

	if (argumentCount - argIndex >= 1) {
		auto status = chakra.JsNumberToDouble_(arguments[argIndex], &textSize);
		THROW_IF_ERROR(status, L"invalid text size");
		argIndex++;
	}

	auto height = (double)DrawUtils::getFontHeight((float)textSize);

	return chakra.toNumber(height);
}

JsValueRef CALLBACK DrawFunctions::getOrigin(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto origin = DrawUtils::getOrigin();
	return scriptMgr.prepareVector3(origin, reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK DrawFunctions::getWindowSize(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto windowSize = g_Data.getGuiData()->windowSize;
	return scriptMgr.prepareVector2(windowSize, reinterpret_cast<ContextObjects*>(callbackState));
}