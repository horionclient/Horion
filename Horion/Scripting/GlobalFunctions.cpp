#include "GlobalFunctions.h"

#include <sstream>

JsValueRef CALLBACK GlobalFunctions::log(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::wstringstream strstream;
	JsValueRef calleString;
	chakra.JsConvertValueToString_(callee, &calleString);
	const wchar_t* coolBoi;
	size_t length1;
	chakra.JsStringToPointer_(calleString, &coolBoi, &length1);
	strstream << coolBoi << L": ";
	for (unsigned int index = 1; index < argumentCount; index++) {
		if (index > 1) {
			strstream << L" ";
		}

		JsValueRef stringValue;
		chakra.JsConvertValueToString_(arguments[index], &stringValue);

		const wchar_t* string;
		size_t length;
		chakra.JsStringToPointer_(stringValue, &string, &length);

		strstream << string;
	}

	logF("%S", strstream.str().c_str());

	return JS_INVALID_REFERENCE;
}
