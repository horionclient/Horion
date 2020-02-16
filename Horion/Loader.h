#pragma once

#include "../Utils/Logger.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>

#include "../Memory/GameData.h"
#include "../Memory/Hooks.h"
#include "../Memory/MinHook.h"
#include "../SDK/CChestBlockActor.h"
#include "../SDK/CClientInstance.h"
#include "../SDK/CClientInstanceScreenModel.h"
#include "../SDK/CEntity.h"
#include "../SDK/CGameMode.h"
#include "../SDK/CPacket.h"
#include "../Utils/HMath.h"
#include "../Utils/Target.h"
#include "../Utils/TextFormat.h"
#include "../Utils/Utils.h"
#include "../include/WinHttpClient.h"
#include "Command/CommandMgr.h"
#include "Config/ConfigManager.h"
#include "Menu/ClickGui.h"
#include "Menu/TabGui.h"
#include "Module/ModuleManager.h"
#include "ImmediateGui.h"
