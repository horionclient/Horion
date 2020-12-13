#include "CClientInstance.h"
#include "../Utils/Utils.h"
#include "../Utils/Logger.h"
#include <cstdarg>

__int64 MinecraftGame::getServerEntries() {
	return Utils::CallVFunc<30, __int64>(this);
}
void C_GuiData::displayClientMessage(std::string *a2) {
	using displayClientMessage = void(__thiscall*)(void*, TextHolder&);  
	static displayClientMessage displayMessageFunc = reinterpret_cast<displayClientMessage>(FindSignature("48 89 5C 24 ?? 55 48 8D 6C 24 ?? 48 81 EC ?? ?? ?? ?? 48 C7 45 4F ?? ?? ?? ?? 33 DB 48 89 5D ?? 88 5D"));

	TextHolder text = TextHolder(*a2);

	if (displayMessageFunc != nullptr)
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

mce::MaterialPtr::MaterialPtr(const std::string &materialName) {

	using materialPtrConst_t = void(__fastcall*)(mce::MaterialPtr *, __int64, const HashedString&); 
	static materialPtrConst_t materialPtrConst = reinterpret_cast<materialPtrConst_t>(FindSignature("48 89 4C 24 ?? 57 48 83 EC ?? 48 C7 44 24 ?? FE FF FF FF 48 89 5C 24 ?? 48 89 74 24 ?? 4C 8B CA"));

	static __int64 renderGroupBase = 0;
	if (renderGroupBase == 0){
		auto sig = FindSignature("48 8D 15 ?? ?? ?? ?? 48 8D 4C 24 ?? E8 ?? ?? ?? ?? 48 8D 54 24 ?? 49 8D 8F") + 3;
		auto off = *reinterpret_cast<int *>(sig);
		renderGroupBase = sig + 4 + off;
	}

	HashedString hashedStr(materialName);
	materialPtrConst(this, renderGroupBase, hashedStr);
}

void mce::Mesh::renderMesh(__int64 screenContext, mce::MaterialPtr *material, size_t numTextures, __int64 **textureArray) {
	struct TextureData {
		size_t numTextures;
		__int64 **texturePtr;
	} data;
	using renderMesh_t = __int64 (*)(mce::Mesh*, __int64, mce::MaterialPtr *, TextureData *);
	static renderMesh_t renderMesh = reinterpret_cast<renderMesh_t>(FindSignature("40 53 55 56 57 41 54 41 55 41 56 41 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 49 8B F1 4D 8B E0 4C"));

	data.numTextures = numTextures;
	data.texturePtr = textureArray;

	renderMesh(this, screenContext + 0x10, material, &data);
}
