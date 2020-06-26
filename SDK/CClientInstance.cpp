#include "CClientInstance.h"
#include "../Utils/Utils.h"
#include "../Utils/Logger.h"
#include <cstdarg>

__int64 MinecraftGame::getServerEntries() {
	return Utils::CallVFunc<30, __int64>(this);
}
void C_GuiData::displayClientMessage(std::string *a2) {
	using displayClientMessage = void(__thiscall*)(void*, TextHolder);  // This signature actually exists 2 times but we got luck that our function is the first in memory
	static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(FindSignature("4C 8B DC 48 ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 49 ?? ?? ?? ?? ?? ?? ?? 33 C0 49 89 43 D8 41 88 43 C8 49"));

	TextHolder text = TextHolder(*a2);

	displayMessageFunc(this, text);
}
void C_GuiData::displayClientMessageF(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	displayClientMessageVA(fmt, arg, true);
	va_end(arg);
}
void C_GuiData::displayClientMessageVA(const char *fmt, va_list lis, bool sendToInjector) {
	auto lengthNeeded = _vscprintf(fmt, lis) + 1;
	if(lengthNeeded >= 300) {
		logF("A message that was %i characters long could not be fitted into the buffer", lengthNeeded);
		return;
	}

	char message[300];
	vsprintf_s(message, 300, fmt, lis);
	std::string msg(message);
	if(sendToInjector)
		Logger::SendToConsoleF(message);
	displayClientMessage(&msg);

}
void C_GuiData::displayClientMessageNoSendF(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);
	displayClientMessageVA(fmt, arg, false);
	va_end(arg);
}
