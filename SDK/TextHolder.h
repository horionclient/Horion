#pragma once

#include <string>

class TextHolder {
public:
	union {
		char inlineText[16];  //0x0000
		char* pText;          //0x0000
	};

	size_t textLength;         //0x0010
	size_t alignedTextLength;  //0x0018

	TextHolder() {
		memset(this, 0, sizeof(TextHolder));
		this->alignedTextLength = 0xF;
	}

	TextHolder(TextHolder const& copy) {
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
	}

	TextHolder& operator=(TextHolder const& copy) {
		freeText();
		memset(this, 0, sizeof(TextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
		return *this;
	}

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			pText = reinterpret_cast<char*>(malloc(size + 1));
			if (pText != 0x0) {
				strcpy_s(pText, size + 1, str.c_str());
			}
		}
	}

	TextHolder(void* ptr, size_t sizeOfData, bool copy) {
		memset(this, 0, sizeof(TextHolder));
		textLength = sizeOfData;
		alignedTextLength = sizeOfData;
		if (textLength < 16)
			memcpy(inlineText, ptr, sizeOfData);
		else {
			if (copy) {
				pText = new char[alignedTextLength + 1];
				memcpy(pText, ptr, sizeOfData);
				pText[alignedTextLength] = 0;
			} else
				pText = reinterpret_cast<char*>(ptr);
		}
			
	}

	~TextHolder() {
		freeText();
		this->textLength = 0;
		this->alignedTextLength = 0xF;
	}

	void freeText() {

		if (alignedTextLength >= 0x10) {
			__int64 v18 = reinterpret_cast<__int64>(this->pText);
			delete((void*)v18);
		} else
			this->inlineText[0] = 0;
	}

	char* getText() {
		if (alignedTextLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	size_t getTextLength() {
		return textLength;
	}

	void setText(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			char* ptr = reinterpret_cast<char*>(malloc(size + 1));
			if (ptr != 0x0) {
				strcpy_s(ptr, size + 1, str.c_str());
			}

			pText = ptr;
		}
	}

	void resetWithoutDelete() {
		memset(this, 0, sizeof(TextHolder));
	}
};

class GamerTextHolder {
public:
	union {
		char inlineText[16];  //0x0000
		char* pText;          //0x0000
	};

	size_t textLength;         //0x0010
	size_t alignedTextLength;  //0x0018

	GamerTextHolder() {
		memset(this, 0, sizeof(GamerTextHolder));
		this->alignedTextLength = 0xF;
	}

	GamerTextHolder(GamerTextHolder const& copy) {
		memset(this, 0, sizeof(GamerTextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
	}

	GamerTextHolder& operator=(GamerTextHolder const& copy) {
		freeText();
		memset(this, 0, sizeof(GamerTextHolder));
		textLength = copy.textLength;
		alignedTextLength = copy.alignedTextLength;
		if (copy.textLength < 16)
			strcpy_s(inlineText, 16, copy.inlineText);
		else {
			size_t size = strlen(copy.pText) + 1;
			pText = reinterpret_cast<char*>(malloc(size));
			if (pText != 0x0 && copy.pText != 0x0) {
				strcpy_s(pText, size, copy.pText);
			}
		}
		return *this;
	}

	GamerTextHolder(std::string str) {
		memset(this, 0, sizeof(GamerTextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			pText = reinterpret_cast<char*>(malloc(size + 1));
			if (pText != 0x0) {
				strcpy_s(pText, size + 1, str.c_str());
			}
		}
	}

	GamerTextHolder(void* ptr, size_t sizeOfData, bool copy) {
		memset(this, 0, sizeof(GamerTextHolder));
		textLength = sizeOfData;
		alignedTextLength = sizeOfData;
		if (textLength < 16)
			memcpy(inlineText, ptr, sizeOfData);
		else {
			if (copy) {
				pText = new char[alignedTextLength + 1];
				memcpy(pText, ptr, sizeOfData);
				pText[alignedTextLength] = 0;
			} else
				pText = reinterpret_cast<char*>(ptr);
		}
	}

	~GamerTextHolder() {
		freeText();
		this->textLength = 0;
		this->alignedTextLength = 0xF;
	}

	void freeText() {
		using mcfree_t = void*(__cdecl*)(void*);

		static mcfree_t mcfree = reinterpret_cast<mcfree_t>(0);
		if (mcfree == nullptr) {
			auto sig = Utils::FindSignature("FF 15 ?? ?? ?? ?? CC E8 ?? ?? ?? ?? 66 0F 6F 05 ?? ?? ?? ?? F3 0F 7F 44 24 50 C6 44 24 40 00 8B 44 24 28");
			int offset = *reinterpret_cast<int*>(sig + 8);
			mcfree = reinterpret_cast<mcfree_t>(sig + 12 + offset);
		}

		if (alignedTextLength >= 0x10) {
			__int64 v18 = reinterpret_cast<__int64>(this->pText);
			if (alignedTextLength + 1 >= 0x1000) {
				v18 = *(__int64*)(reinterpret_cast<__int64>(this->pText) - 8);

				if ((unsigned __int64)(reinterpret_cast<__int64>(this->pText) - v18 - 8) > 0x1F)
					__debugbreak();
			}
			mcfree((void*)v18);
		} else
			this->inlineText[0] = 0;
	}

	char* getText() {
		if (alignedTextLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	size_t getTextLength() {
		return textLength;
	}

	void setText(std::string str) {
		memset(this, 0, sizeof(GamerTextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			char* ptr = reinterpret_cast<char*>(malloc(size + 1));
			if (ptr != 0x0) {
				strcpy_s(ptr, size + 1, str.c_str());
			}

			pText = ptr;
		}
	}

	void resetWithoutDelete() {
		memset(this, 0, sizeof(GamerTextHolder));
	}
};
