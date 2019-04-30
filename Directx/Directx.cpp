#include "Directx.h"

IDXGISwapChain     * pDummySwapChain = nullptr;
ID3D11Device     * pDummyDevice = nullptr;
ID3D11DeviceContext  * pDummyContext = nullptr;

IDXGISwapChain     * pSwapChain = nullptr;
ID3D11Device     * pDevice = nullptr;
ID3D11DeviceContext  * pContext = nullptr;



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

	/*pDummySwapChain = nullptr;
	pDummyDevice = nullptr;
	pDummyContext = nullptr;*/

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
		//pSwapchainTable = *reinterpret_cast<void***>(pDummySwapChain);
		//logF("Swap Chain ptr  = %llX", pDummySwapChain);
		//logF("Swap Chain Table = %llX", pSwapchainTable);
	}

	if (pDeviceTable)
	{
		memcpy(pDeviceTable, *reinterpret_cast<void***>(pDummyDevice), Size_Device);
		//pDeviceTable = *reinterpret_cast<void***>(pDummyDevice);
		//logF("Device ptr  = %llX", pDummyDevice);
		//logF("Device Vtable = %llX", pDeviceTable);
	}

	if (pContextTable)
	{
		memcpy(pContextTable, *reinterpret_cast<void***>(pDummyContext), Size_Context);
		//pContextTable = *reinterpret_cast<void***>(pDummyContext);
		//logF("Context ptr  = %llX", pDummyContext);
		//logF("Device context Vtabl = %llX", pContextTable);
	}

	pDummySwapChain->Release();
	pDummyDevice->Release();
	pDummyContext->Release();

	return true;
}


void* getSwapChain()
{
	if (GetD3D11SwapchainDeviceContext(SwapChain, sizeof(SwapChain), Device, sizeof(Device), Context, sizeof(Context)))
	{
		/*logF("Device Pointer = %llX", SwapChain);

		logF("Device context Pointer = %llX", Context);

		logF("Swap Chain Vtable = %llX", ptr);*/

		logF("YEET");
		pSwapChain = reinterpret_cast<IDXGISwapChain*>(0x231435A6A00);
		if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&pDevice)))
		{
			logF("Failed");
			return false;
		}
		//pDevice2 = reinterpret_cast<ID3D11Device*>(0x23143AE6870);

		pDevice->GetImmediateContext(&pContext);

		// Make sure this is done only one time!
		logF("SwapChain ptr  = %llX", pSwapChain);
		logF("pDevice ptr  = %llX", pDevice);
		logF("Context ptr  = %llX", pContext);
		return SwapChain;


		//hook stuff using the dumped addresses
	}
	return 0;
}
void Draw()
{

	static ID3D11Buffer*			m_pVertexBuffer = 0;
	//static ID3D11InputLayout*		m_pInputLayout = 0;
	//static ID3DX11EffectTechnique* m_pTechnique;

	//D3DX11_PASS_DESC passDesc;

	D3D11_INPUT_ELEMENT_DESC lineRectLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	//if (FAILED(m_pTechnique->GetPassByIndex(0)->GetDesc(&passDesc)))
		//return false;

	//if (FAILED(pDevice->CreateInputLayout(lineRectLayout, sizeof(lineRectLayout) / sizeof(lineRectLayout[0]), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &m_pInputLayout)))
		//return false;

	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = 31 * sizeof(COLOR_VERTEX);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	if (m_pVertexBuffer == 0 && FAILED(pDevice->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer)))
		logF("pDevice->CreateBuffer failed");

	float x0 = 100;
	float y0 = 100;
	float x1 = 500;
	float y1 = 500;

	float r = 1;
	float g = 0.5f;
	float b = 0.4f;
	float a = 1;

	UINT viewportNumber = 1;

	D3D11_VIEWPORT vp;

	pContext->RSGetViewports(&viewportNumber, &vp);

	float xx0 = 2.0f * (x0 - 0.5f) / vp.Width - 1.0f;
	float yy0 = 1.0f - 2.0f * (y0 - 0.5f) / vp.Height;
	float xx1 = 2.0f * (x1 - 0.5f) / vp.Width - 1.0f;
	float yy1 = 1.0f - 2.0f * (y1 - 0.5f) / vp.Height;

	COLOR_VERTEX* v = NULL;

	D3D11_MAPPED_SUBRESOURCE mapData;

	if (FAILED(pContext->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapData)))
		logF("pContext->Map failed");
	//return;

	v = (COLOR_VERTEX*)mapData.pData;


	v[0].Position.x = xx0;
	v[0].Position.y = yy0;
	v[0].Position.z = 0;
	v[0].Color.r = ((FLOAT)r / 255.0f);
	v[0].Color.g = ((FLOAT)g / 255.0f);
	v[0].Color.b = ((FLOAT)b / 255.0f);
	v[0].Color.a = ((FLOAT)a / 255.0f);

	v[1].Position.x = xx1;
	v[1].Position.y = yy1;
	v[1].Position.z = 0;
	v[1].Color.r = ((FLOAT)r / 255.0f);
	v[1].Color.g = ((FLOAT)g / 255.0f);
	v[1].Color.b = ((FLOAT)b / 255.0f);
	v[1].Color.a = ((FLOAT)a / 255.0f);

	pContext->Unmap(m_pVertexBuffer, NULL);

	//pContext->IASetInputLayout(0);


	UINT Stride = sizeof(COLOR_VERTEX);
	UINT Offset = 0;

	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	//for (UINT p = 0; p < techDesc.Passes; ++p)
	{


		pContext->Draw(2, 0);
	}
}

