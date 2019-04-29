#include "Directx.h"



void * SwapChain[18];
void * Device[40];
void * Context[108];

//Create temporary DeviceAndSwapChain

bool GetD3D11SwapchainDeviceContext(void ** pSwapchainTable, size_t Size_Swapchain, void ** pDeviceTable, size_t Size_Device, void ** pContextTable, size_t Size_Context)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc{ 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetForegroundWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	IDXGISwapChain        * pDummySwapChain = nullptr;
	ID3D11Device        * pDummyDevice = nullptr;
	ID3D11DeviceContext    * pDummyContext = nullptr;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, NULL, &pDummyContext)))
	{
		swapChainDesc.Windowed = TRUE;
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &pDummySwapChain, &pDummyDevice, NULL, &pDummyContext)))
		{
			return false;
		}
	}

	if (pSwapchainTable)
	{
		memcpy(pSwapchainTable, *reinterpret_cast<void***>(pDummySwapChain), Size_Swapchain);
		logF("Swap Chain Table = %llX", pSwapchainTable);
	}

	if (pDeviceTable)
	{
		memcpy(pDeviceTable, *reinterpret_cast<void***>(pDummyDevice), Size_Device);
		logF("Device Vtable = %llX",pDeviceTable);
	}

	if (pContextTable)
	{
		memcpy(pContextTable, *reinterpret_cast<void***>(pDummyContext), Size_Context);
		logF("Device context Vtable = %llX", pContextTable);
	}

	pDummySwapChain->Release();
	pDummyDevice->Release();
	pDummyContext->Release();

	return true;
}


void DirectX_Yeet()
{
	if (GetD3D11SwapchainDeviceContext(SwapChain, sizeof(SwapChain), Device, sizeof(Device), Context, sizeof(Context)))
	{
		/*logF("Device Pointer = %llX", SwapChain);
		
		logF("Device context Pointer = %llX", Context);
		
		logF("Swap Chain Vtable = %llX", ptr);*/
		
		logF("YEET");
		//hook stuff using the dumped addresses
	}
}