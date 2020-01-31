#pragma once

#include "../Utils/Utils.h"

class C_ClientInstanceScreenModel {
public:
	void sendChatMessage(char* message) {
		using sendChatMessage = void(__fastcall*)(void*, char*);
		static sendChatMessage sendChatMessageFunc = reinterpret_cast<sendChatMessage>(FindSignature("56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ?? ?? ?? ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 D8 ?? ?? ?? ?? 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 85 ?? ?? ?? ?? 48 89 54 24 ??") - 2);

		if (sendChatMessageFunc != 0x0)
			sendChatMessageFunc(this, (char*)std::string(message).c_str());
	}
};
