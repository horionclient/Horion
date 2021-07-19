#pragma once

#include "../Utils/Utils.h"

class C_ClientInstanceScreenModel {
public:
	void sendChatMessage(char* message) {
		using sendChatMessage = void(__fastcall*)(void*, char*);
		static sendChatMessage sendChatMessageFunc = reinterpret_cast<sendChatMessage>(FindSignature("48 89 5C 24 18 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 E0 F6 FF FF 48 81 EC 20 0A 00 00 48 8B 05 4A 30 DE 00 48 33 C4 48 89 85 10 09 00 00 45 0F B6 E0 44") - 2);

		if (sendChatMessageFunc != 0x0)
			sendChatMessageFunc(this, (char*)std::string(message).c_str());
	}
};
