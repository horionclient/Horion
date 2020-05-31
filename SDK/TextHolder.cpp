#include "TextHolder.h"

#include <Windows.h>

void GamerTextHolder::copyFrom(TextHolder *copy) {
	typedef void*(__cdecl * _malloctype)(size_t);

	static auto _malloc = reinterpret_cast<_malloctype>(GetProcAddress(GetModuleHandleA("api-ms-win-crt-heap-l1-1-0"), "malloc"));

	deleteText();
	memset(this, 0, sizeof(TextHolder));
	textLength = copy->textLength;
	alignedTextLength = copy->alignedTextLength;
	if (copy->textLength < 16) {
		memcpy(inlineText, copy->inlineText, 16);
	}else {
		size_t size = copy->textLength;

		if (size + 1 >= 0x1000)
			size += 8;

		pText = reinterpret_cast<char*>(_malloc(size + 1));

		if (size + 1 >= 0x1000) {
			*reinterpret_cast<char**>(pText) = pText;
			pText += 8;
		}

		if (pText != 0x0 && copy->pText != 0x0) {
			memcpy(pText, copy->pText, textLength);
			pText[textLength] = 0;
		}
	}
}
void GamerTextHolder::deleteText() {
	typedef void(__cdecl * _freetype)(void*);
	static auto _free = reinterpret_cast<_freetype>(GetProcAddress(GetModuleHandleA("api-ms-win-crt-heap-l1-1-0"), "free"));

	if (textLength >= 16 && pText != nullptr) {
		if (textLength + 1 >= 0x1000) {
			pText = *reinterpret_cast<char**>(reinterpret_cast<__int64>(pText) - 8);
		}
		_free(pText);
	}
}
