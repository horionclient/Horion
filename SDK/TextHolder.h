#pragma once

#pragma pack(push,4)
__declspec(align(4)) 
class TextHolder
{	
public:

	union {
		char inlineText[16]; //0x0000 
		char *pText; //0x0000 
	};

	int textLength; //0x0010 

	TextHolder(std::string str) {
		memset(this, 0, sizeof(TextHolder));
		if (str.size() < 16)
			strcpy_s(inlineText, 16, str.c_str());
		else {
			size_t size = str.size();
			char* ptr = reinterpret_cast<char*>(malloc(size));
			strcpy_s(ptr, size, str.c_str());
		}
	}

	char* getText() {
		if (textLength < 16)
			return this->inlineText;
		else
			return this->pText;
	}

	int getTextLength() {
		return textLength;
	}
};

#pragma pack(pop)