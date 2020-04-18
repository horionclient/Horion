#pragma once
#include "DllHelper.h"
#include "../include/chakra/ChakraCore.h"
#include <string>

#define DECL_API(f) decltype(f)* f##_ = _dll[#f]

class ChakraApi {
	DllHelper _dll{"Chakra.dll"};

public:
	DECL_API(JsCreateRuntime);
	DECL_API(JsCreateContext);
	DECL_API(JsSetCurrentContext);
	DECL_API(JsDisposeRuntime);
	DECL_API(JsRunScript);
	DECL_API(JsConvertValueToString);
	DECL_API(JsStringToPointer);
	DECL_API(JsSetRuntimeMemoryLimit);
	DECL_API(JsCreateObject);
	DECL_API(JsGetGlobalObject);
	DECL_API(JsGetPropertyIdFromName);
	DECL_API(JsPointerToString);
	DECL_API(JsSetProperty);
	DECL_API(JsCreateFunction);
	DECL_API(JsCreateString);
	DECL_API(JsHasException);
	DECL_API(JsGetAndClearException);
	DECL_API(JsCollectGarbage);
	DECL_API(JsAddRef);
	DECL_API(JsRelease);
	DECL_API(JsGetNullValue);
	DECL_API(JsCreateExternalObject);
	DECL_API(JsGetCurrentContext);
	DECL_API(JsGetExternalData);
	DECL_API(JsGetUndefinedValue);
	DECL_API(JsStrictEquals);
	DECL_API(JsGetValueType);
	DECL_API(JsHasExternalData);
	DECL_API(JsBoolToBoolean);
	DECL_API(JsCreateError);
	DECL_API(JsSetException);
	DECL_API(JsCreateTypeError);
	DECL_API(JsDefineProperty);
	DECL_API(JsGetTrueValue);
	DECL_API(JsGetFalseValue);
	DECL_API(JsDoubleToNumber);

	void throwException(std::wstring error) {
		JsValueRef errorValue;
		JsValueRef errorObject;
		this->JsPointerToString_(error.c_str(), error.length(), &errorValue);
		this->JsCreateError_(errorValue, &errorObject);
		this->JsSetException_(errorObject);
	}

	void throwTypeException(std::wstring error) {
		JsValueRef errorValue;
		JsValueRef errorObject;
		this->JsPointerToString_(error.c_str(), error.length(), &errorValue);
		this->JsCreateTypeError_(errorValue, &errorObject);
		this->JsSetException_(errorObject);
	}

	JsValueRef toNumber(double doubleVal) {
		JsValueRef ref = JS_INVALID_REFERENCE;
		this->JsDoubleToNumber_(doubleVal, &ref);
		return ref;
	}

	void defineFunction(JsValueRef object, const wchar_t* callbackName, JsNativeFunction function, void* callbackState = nullptr) {
		JsPropertyIdRef propertyId;
		this->JsGetPropertyIdFromName_(callbackName, &propertyId);

		JsValueRef functionRef;
		this->JsCreateFunction_(function, callbackState, &functionRef);

		this->JsSetProperty_(object, propertyId, functionRef, true);
	}

	void addPropertyToObj(JsValueRef obj, const wchar_t* name, JsValueRef prop) {
		JsPropertyIdRef propertyId;
		this->JsGetPropertyIdFromName_(name, &propertyId);
		this->JsSetProperty_(obj, propertyId, prop, true);
	}

	bool defineValueProp(JsValueRef object, const wchar_t* name, JsValueRef value, bool changeable) {
		JsPropertyIdRef namePropertyId;
		this->JsGetPropertyIdFromName_(name, &namePropertyId);

		JsValueRef propertyDesc;
		this->JsCreateObject_(&propertyDesc);

		JsValueRef jsTrue;
		this->JsGetTrueValue_(&jsTrue);
		JsValueRef jsFalse;
		this->JsGetFalseValue_(&jsFalse);

		addPropertyToObj(propertyDesc, L"value", value);
		addPropertyToObj(propertyDesc, L"writable", jsFalse);
		addPropertyToObj(propertyDesc, L"enumerable", jsTrue);
		addPropertyToObj(propertyDesc, L"configurable", jsFalse);

		bool result;
		this->JsDefineProperty_(object, namePropertyId, propertyDesc, &result);
		return result;
	}

	bool defineProp(JsValueRef object, const wchar_t* name, JsNativeFunction getter, JsNativeFunction setter) {
		JsPropertyIdRef namePropertyId;
		this->JsGetPropertyIdFromName_(name, &namePropertyId);

		JsValueRef propertyDesc;
		this->JsCreateObject_(&propertyDesc);

		JsValueRef get = NULL, set = NULL;
		if (getter != nullptr)
			this->JsCreateFunction_(getter, 0, &get);
		if (setter != nullptr)
			this->JsCreateFunction_(setter, 0, &set);

		JsValueRef jsTrue;
		this->JsGetTrueValue_(&jsTrue);
		JsValueRef jsFalse;
		this->JsGetFalseValue_(&jsFalse);

		if (getter != nullptr)
			addPropertyToObj(propertyDesc, L"get", get);
		if (setter != nullptr)
			addPropertyToObj(propertyDesc, L"set", set);
		addPropertyToObj(propertyDesc, L"enumerable", jsTrue);
		addPropertyToObj(propertyDesc, L"configurable", jsFalse);

		bool result;
		this->JsDefineProperty_(object, namePropertyId, propertyDesc, &result);
		return result;
	}

	bool isNullOrUndefined(JsValueRef ref) {
		JsValueType type = JsUndefined;
		this->JsGetValueType_(ref, &type);
		return type == JsUndefined || type == JsNull;
	}

	std::wstring valueToString(JsValueRef value) {
		JsValueRef resultJSString;
		auto err = JsConvertValueToString_(value, &resultJSString);
		if (err == JsNoError) {
			const wchar_t* resultWC;
			size_t stringLength;
			err = JsStringToPointer_(resultJSString, &resultWC, &stringLength);

			if (err == JsNoError)
				return std::wstring(resultWC, stringLength);
			else
				return std::wstring(L"error#") + std::to_wstring(err);
		} else
			return std::wstring(L"error#") + std::to_wstring(err);
	}
};