#pragma once

class TextHolder
{	
public:

	union {
		char inlineText[16]; //0x0000 
		char *pText; //0x0000 
	};

	size_t textLength; //0x0010 
	size_t alignedTextLength; //0x0018

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		textLength = str.size();
		alignedTextLength = textLength | 0xF;
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			char* ptr = reinterpret_cast<char*>(malloc(size + 1));
			strcpy_s(ptr, size + 1, str.c_str());
			pText = ptr;
		}
	}

	~TextHolder() {
		if (textLength >= 16 && pText != nullptr) {
			free(pText);
		}
	}

	char* getText() {
		if (textLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	size_t getTextLength() {
		return textLength;
	}
};