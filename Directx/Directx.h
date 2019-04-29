#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "../Utils/TextFormat.h"
bool GetD3D11SwapchainDeviceContext(void ** pSwapchainTable, size_t Size_Swapchain, void ** pDeviceTable, size_t Size_Device, void ** pContextTable, size_t Size_Context);
void DirectX_Yeet();