#include "Vector2Functions.h"

std::optional<vec2_t> Vector2Functions::getVec2FromValue(JsValueRef ref) {
	JsValueType type;
	auto err = chakra.JsGetValueType_(ref, &type);
	if (type != JsObject || err != JsNoError)
		return std::optional<vec2_t>();

	bool hasExternalData = false;
	err = chakra.JsHasExternalData_(ref, &hasExternalData);
	if (!hasExternalData || err != JsNoError)
		return std::optional<vec2_t>();

	JVector2* vecInfo;
	err = chakra.JsGetExternalData_(ref, reinterpret_cast<void**>(&vecInfo));
	if (vecInfo->dataType != Vector2DataType || err != JsNoError)
		return std::optional<vec2_t>();
	return std::optional<vec2_t>(vecInfo->vec);
}

std::optional<vec2_t> Vector2Functions::getVec2FromArguments(JsValueRef* args, int argCount, int* nextArg) {
	if (argCount <= 0)
		return std::optional<vec2_t>();

	auto vec = Vector2Functions::getVec2FromValue(args[0]);

	if (vec.has_value()){
		if(nextArg)
			*nextArg += 1;
		return vec.value();
	}

	if (argCount < 2)
		return std::optional<vec2_t>();

	bool isValid = true;
	JsValueType type;
	for (int i = 0; i < 2; i++) {
		auto err = chakra.JsGetValueType_(args[i], &type);
		if (err != JsNoError || type != JsNumber) {
			isValid = false;
			break;
		}
	}

	if (!isValid)
		return std::optional<vec2_t>();

	double x, y;
	int err = 0;
	err |= (int)chakra.JsNumberToDouble_(args[0], &x);
	err |= (int)chakra.JsNumberToDouble_(args[1], &y);
	if ((JsErrorCode)err != JsNoError)
		return std::optional<vec2_t>();

	if(nextArg)
		*nextArg += 2;

	return std::optional<vec2_t>(vec2_t((float)x, (float)y));
}

JsValueRef CALLBACK Vector2Functions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	JsValueRef isValidBoolean;
	chakra.JsBoolToBoolean_(vecOpt.has_value(), &isValidBoolean);

	return isValidBoolean;
}

JsValueRef CALLBACK Vector2Functions::getX(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toNumber(vecOpt->x);
}

JsValueRef CALLBACK Vector2Functions::getY(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}

	return chakra.toNumber(vecOpt->y);
}

JsValueRef CALLBACK Vector2Functions::toString(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		chakra.throwTypeException(L"Vector is invalid!");
		return JS_INVALID_REFERENCE;
	}
	auto vec = vecOpt.value();

	wchar_t name[80];
	int length = swprintf_s(name, L"Vec2(x=%.2f, y=%.2f)", vec.x, vec.y);
	JsValueRef ref;
	chakra.JsPointerToString_(name, length, &ref);
	return ref;
}

JsValueRef CALLBACK Vector2Functions::constructor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	if (argumentCount != 3) {
		chakra.throwTypeException(L"Vec2 constructor needs 2 parameters!");
		return JS_INVALID_REFERENCE;
	}

	double x, y;
	chakra.JsNumberToDouble_(arguments[1], &x);
	chakra.JsNumberToDouble_(arguments[2], &y);

	return scriptMgr.prepareVector2(vec2_t((float)x, (float)y), reinterpret_cast<ContextObjects*>(callbackState));
}

JsValueRef CALLBACK Vector2Functions::add(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		THROW(L"Vector is invalid!");
	}

	if (argumentCount < 2) {
		THROW(L"At least 1 argument needed");
	}

	switch(argumentCount - 1){
		case 1: {
			// either adding a Vec2 or a float to all 2 components
			JsValueType type;
			chakra.JsGetValueType_(arguments[1], &type);

			if(type == JsNumber){
				double val = 0;
				if(chakra.JsNumberToDouble_(arguments[1], &val) != JsNoError){
					THROW(L"Argument 1 not a valid number");
				}

				return scriptMgr.prepareVector2(vecOpt->add((float)val), reinterpret_cast<ContextObjects*>(callbackState));
			}else if(type == JsObject){
				auto oVec = Vector2Functions::getVec2FromValue(arguments[1]);
				if (!oVec.has_value()) {
					THROW(L"Supplied Vector is invalid!");
				}

				return scriptMgr.prepareVector2(vecOpt->add(*oVec), reinterpret_cast<ContextObjects*>(callbackState));
			}
		} break;
		case 2: {
			// adding with 2 individual floats
			vec2_t oVec;
			for(int i = 0; i < 2; i++){
				JsValueType type;
				chakra.JsGetValueType_(arguments[1 + i], &type);
				if(type != JsNumber){
					THROW(L"Invalid argument supplied");
				}
				double val = 0;
				if(chakra.JsNumberToDouble_(arguments[1 + i], &val) != JsNoError){
					THROW(L"Argument not a valid number");
				}

				oVec.floatArr[i] = (float)val;
			}

			return scriptMgr.prepareVector2(vecOpt->add(oVec), reinterpret_cast<ContextObjects*>(callbackState));
		} break;
	}

	THROW(L"Invalid arguments");
}

JsValueRef CALLBACK Vector2Functions::sub(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		THROW(L"Vector is invalid!");
	}

	if (argumentCount < 2) {
		THROW(L"At least 1 argument needed");
	}

	switch(argumentCount - 1){
	case 1: {
		// either subtracting a Vec2 or a float to all 2 components
		JsValueType type;
		chakra.JsGetValueType_(arguments[1], &type);

		if(type == JsNumber){
			double val = 0;
			if(chakra.JsNumberToDouble_(arguments[1], &val) != JsNoError){
				THROW(L"Argument 1 not a valid number");
			}

			return scriptMgr.prepareVector2(vecOpt->sub((float)val), reinterpret_cast<ContextObjects*>(callbackState));
		}else if(type == JsObject){
			auto oVec = Vector2Functions::getVec2FromValue(arguments[1]);
			if (!oVec.has_value()) {
				THROW(L"Supplied Vector is invalid!");
			}

			return scriptMgr.prepareVector2(vecOpt->sub(*oVec), reinterpret_cast<ContextObjects*>(callbackState));
		}
	} break;
	case 2: {
		// adding with 2 individual floats
		vec2_t oVec;
		for(int i = 0; i < 2; i++){
			JsValueType type;
			chakra.JsGetValueType_(arguments[1 + i], &type);
			if(type != JsNumber){
				THROW(L"Invalid argument supplied");
			}
			double val = 0;
			if(chakra.JsNumberToDouble_(arguments[1 + i], &val) != JsNoError){
				THROW(L"Argument not a valid number");
			}

			oVec.floatArr[i] = (float)val;
		}

		return scriptMgr.prepareVector2(vecOpt->sub(oVec), reinterpret_cast<ContextObjects*>(callbackState));
	} break;
	}

	THROW(L"Invalid arguments");
}

JsValueRef CALLBACK Vector2Functions::div(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		THROW(L"Vector is invalid!");
	}

	if (argumentCount < 2) {
		THROW(L"At least 1 argument needed");
	}

	switch (argumentCount - 1) {
	case 1: {
		// either adding a Vec2 or a float to all 2 components
		JsValueType type;
		chakra.JsGetValueType_(arguments[1], &type);

		if (type == JsNumber) {
			double val = 0;
			if (chakra.JsNumberToDouble_(arguments[1], &val) != JsNoError) {
				THROW(L"Argument 1 not a valid number");
			}

			return scriptMgr.prepareVector2(vecOpt->div((float)val), reinterpret_cast<ContextObjects*>(callbackState));
		} else if (type == JsObject) {
			auto oVec = Vector2Functions::getVec2FromValue(arguments[1]);
			if (!oVec.has_value()) {
				THROW(L"Supplied Vector is invalid!");
			}

			return scriptMgr.prepareVector2(vecOpt->div(*oVec), reinterpret_cast<ContextObjects*>(callbackState));
		}
	} break;
	case 2: {
		// adding with 2 individual floats
		vec2_t oVec;
		for (int i = 0; i < 2; i++) {
			JsValueType type;
			chakra.JsGetValueType_(arguments[1 + i], &type);
			if (type != JsNumber) {
				THROW(L"Invalid argument supplied");
			}
			double val = 0;
			if (chakra.JsNumberToDouble_(arguments[1 + i], &val) != JsNoError) {
				THROW(L"Argument not a valid number");
			}

			oVec.floatArr[i] = (float)val;
		}

		return scriptMgr.prepareVector2(vecOpt->div(oVec), reinterpret_cast<ContextObjects*>(callbackState));
	} break;
	}

	THROW(L"Invalid arguments");
}


JsValueRef CALLBACK Vector2Functions::mul(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto vecOpt = Vector2Functions::getVec2FromValue(arguments[0]);
	if (!vecOpt.has_value()) {
		THROW(L"Vector is invalid!");
	}

	if (argumentCount < 2) {
		THROW(L"At least 1 argument needed");
	}

	switch (argumentCount - 1) {
	case 1: {
		// either adding a Vec2 or a float to all 2 components
		JsValueType type;
		chakra.JsGetValueType_(arguments[1], &type);

		if (type == JsNumber) {
			double val = 0;
			if (chakra.JsNumberToDouble_(arguments[1], &val) != JsNoError) {
				THROW(L"Argument 1 not a valid number");
			}

			return scriptMgr.prepareVector2(vecOpt->mul((float)val), reinterpret_cast<ContextObjects*>(callbackState));
		} else if (type == JsObject) {
			auto oVec = Vector2Functions::getVec2FromValue(arguments[1]);
			if (!oVec.has_value()) {
				THROW(L"Supplied Vector is invalid!");
			}

			return scriptMgr.prepareVector2(vecOpt->mul(*oVec), reinterpret_cast<ContextObjects*>(callbackState));
		}
	} break;
	case 2: {
		// adding with 2 individual floats
		vec2_t oVec;
		for (int i = 0; i < 2; i++) {
			JsValueType type;
			chakra.JsGetValueType_(arguments[1 + i], &type);
			if (type != JsNumber) {
				THROW(L"Invalid argument supplied");
			}
			double val = 0;
			if (chakra.JsNumberToDouble_(arguments[1 + i], &val) != JsNoError) {
				THROW(L"Argument not a valid number");
			}

			oVec.floatArr[i] = (float)val;
		}

		return scriptMgr.prepareVector2(vecOpt->mul(oVec), reinterpret_cast<ContextObjects*>(callbackState));
	} break;
	}

	THROW(L"Invalid arguments");
}