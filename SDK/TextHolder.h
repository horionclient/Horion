#pragma once

class TextHolder
{
public:
private:
	union {
		char inlineText[16]; //0x0000 
		char *pText; //0x0000 
	};

	int textLength; //0x0010 
public:
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