#include "GlobalFunctions.h"

#include <sstream>

JsValueRef CALLBACK GlobalFunctions::log(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::wstringstream strstream;
	/*JsValueRef calleString;
	chakra.JsConvertValueToString_(callee, &calleString);
	const wchar_t* coolBoi;
	size_t length1;
	chakra.JsStringToPointer_(calleString, &coolBoi, &length1);
	strstream << coolBoi << L": ";*/
	for (unsigned int index = 1; index < argumentCount; index++) {
		if (index > 1) {
			strstream << L" ";
		}

		auto string = chakra.valueToString(arguments[index]);

		strstream << string;
	}

	#ifdef _DEBUG
	auto string = strstream.str();
	Logger::WriteBigLogFileF(string.size() + 11, "Script: %S", string.c_str());
	#else
	
	auto obj = reinterpret_cast<ContextObjects*>(callbackState);
	if (obj->scriptInstance == nullptr) {
		GameData::log("[inline]: %S", strstream.str().c_str());
	} else {
		GameData::log("[script]: %S", strstream.str().c_str());
	}
	

	#endif

	return JS_INVALID_REFERENCE;
}