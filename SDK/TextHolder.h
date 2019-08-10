#pragma once

#include <string>

class TextHolder
{	
public:

	union {
		char inlineText[16]; //0x0000 
		char *pText; //0x0000 
	};

	size_t textLength; //0x0010 
	size_t alignedTextLength; //0x0018

	TextHolder() {
		memset(this, 0, sizeof(TextHolder));
	}

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			pText = reinterpret_cast<char*>(malloc(alignedTextLength + 1));
			if (pText != 0x0) {
				strcpy_s(pText, size + 1, str.c_str());
			}
		}
	}

	~TextHolder() {
		if (textLength >= 16 && pText != nullptr) {
			memset(pText, 0x0, alignedTextLength + 1);
			free(pText);
		}
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
			char* ptr = reinterpret_cast<char*>(malloc(alignedTextLength + 1));
			if (ptr != 0x0) {
				strcpy_s(ptr, size + 1, str.c_str());
			}

			pText = ptr;
		}
	}
};