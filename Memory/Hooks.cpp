#include "Hooks.h"

Hooks    g_Hooks;

void Hooks::Init()
{
	logF("Initting Hooks");
	// GameMode::tick Signature
	// CC 8B 41 ?? 89 41 ?? C3
	//    ^^ Function starts here
	void* func = reinterpret_cast<void*>(Utils::FindSignature("CC 8B 41 ?? 89 41 ?? C3") + 1);
	g_Hooks.gameMode_tickHook = std::make_unique<FuncHook>(func, Hooks::GameMode_tick);

	// ChatScreenController::_sendChatMessage
	// 40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85
	// ^^ 
	void* _sendChatMessage = reinterpret_cast<void*>(Utils::FindSignature("40 57 48 83 EC 20 48 83 B9 ?? ?? ?? ?? 00 48 8B F9 0F 85"));
	g_Hooks.chatScreen_sendMessageHook = std::make_unique<FuncHook>(_sendChatMessage, Hooks::ChatScreenController_sendChatMessage);

	// IDXGISwapChain::present
	// using vtable found with dummy thing
	void** swapChainVtable = static_cast<void**>(getSwapChain());
	void* presentFunc = swapChainVtable[8];
	g_Hooks.d3d11_presentHook = std::make_unique<FuncHook>(presentFunc, Hooks::d3d11_present);

	logF("Hooks hooked");
}

void Hooks::Restore()
{
	g_Hooks.gameMode_tickHook->Restore();
	g_Hooks.chatScreen_sendMessageHook->Restore();
	g_Hooks.d3d11_presentHook->Restore();
}

void __fastcall Hooks::GameMode_tick(C_GameMode * _this)
{
	static auto oTick = g_Hooks.gameMode_tickHook->GetOriginal<GameMode_tick_t>();
	oTick(_this); // Call Original Func
	GameData::updateGameData(_this);
}

void __fastcall Hooks::ChatScreenController_sendChatMessage(uint8_t * _this)
{
	static auto oSendMessage = g_Hooks.chatScreen_sendMessageHook->GetOriginal<ChatScreen_sendChatMessage_t>();
	uintptr_t* idk = reinterpret_cast<uintptr_t*>(_this + 0x688);
	if (*idk) {
		char* message = reinterpret_cast<char*>(_this + 0x678);
		if (*reinterpret_cast<__int64*>(_this + 0x690) >= 0x10)
			message = *reinterpret_cast<char**>(message);
		__int64* v6 = reinterpret_cast<__int64*>(_this + 0x690);

		if (*message == '.') {
			logF("Command: %s", message);
			*message = 0x0; // Remove command in textbox
			*v6 = 0x0;
			*idk = 0x0;
			return;
		}
	}
	oSendMessage(_this);
}

HRESULT __stdcall Hooks::d3d11_present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static auto oPresent = g_Hooks.d3d11_presentHook->GetOriginal<d3d11_present_t>();
	logF("d3d11_present");
	return oPresent(pSwapChain, SyncInterval, Flags);
}