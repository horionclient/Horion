#pragma once

#include "../Utils/Logger.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "../Memory/SlimMem.h"
#include "../Utils/Utils.h"
#include "../Memory/Offsets.h"
#include "../Utils/HMath.h"
#include "../SDK/CEntity.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CClientInstance.h"
#include "../SDK/CClientInstanceScreenModel.h"
#include "../Memory/MinHook.h"
#include "../SDK/CPacket.h"
#include "../SDK/CChestBlockActor.h"
#include "../Memory/Hooks.h"
#include "../Memory/GameData.h"
#include "../Utils/TextFormat.h"
#include "../Directx/Directx.h"
#include "Module/ModuleManager.h"
#include "Command/CommandMgr.h"
#include "Config/ConfigManager.h"
#include "Menu/TabGui.h"