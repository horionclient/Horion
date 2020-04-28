//
// Game.cpp
//

#include "Game.h"
#include "../../resource.h"

Game GameWnd;


using namespace DirectX;
using namespace DirectX::SimpleMath;

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Init(IDXGISwapChain* pSwapChain ,HMODULE mod) {
  /* m_window = GetWindow;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);*/

    m_dllModule = mod;
    m_swapChain = pSwapChain;
	m_swapChain->GetDevice(__uuidof(ID3D11Device), (void**)&m_d3dDevice);

	m_d3dDevice->GetImmediateContext(&m_d3dContext);


	//m_d3dContext->OMGetRenderTargets(1, &m_renderTargetView, nullptr);

    UpdateDevice();

    UpdateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
}


// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
}

// Draws the scene.
void Game::Render()
{
    Clear();

    // TODO: Add your rendering code here.
	m_spriteBatch->Begin();

	m_spriteBatch->Draw(m_texture.Get(), m_screenPos, nullptr, Colors::White,0.f, m_origin);

	m_spriteBatch->End();

   // Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
  // m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(),Colors::Transparent);
     m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
	OnDeviceLost();
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    UpdateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::UpdateDevice()
{
    // TODO: Initialize device dependent objects here (independent of window size).

    m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());

	ComPtr<ID3D11Resource> resource;

	HRSRC hResource = FindResourceA(m_dllModule, MAKEINTRESOURCEA(IDB_CAT1),"PNG");  // substitute RESOURCE_ID and RESOURCE_TYPE.
	HGLOBAL hMemory = LoadResource(m_dllModule, hResource);
	DWORD dwSize = SizeofResource(m_dllModule, hResource);
	LPVOID lpAddress = LockResource(hMemory);

	 DX::ThrowIfFailed(CreateWICTextureFromMemory(m_d3dDevice.Get(),(unsigned char*)lpAddress, dwSize, resource.GetAddressOf(), m_texture.ReleaseAndGetAddressOf()));

     FreeResource(hMemory);

	ComPtr<ID3D11Texture2D> cat;
	resource.As(&cat);

	CD3D11_TEXTURE2D_DESC catDesc;
	cat->GetDesc(&catDesc);

	m_origin.x = float(catDesc.Width / 2);
	m_origin.y = float(catDesc.Height / 2);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::UpdateResources()
{
	
	ComPtr<ID3D11Texture2D> backBuffer;
	DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	D3D11_TEXTURE2D_DESC backBufferDesc = {0};
	backBuffer->GetDesc(&backBufferDesc);

	m_outputWidth = backBufferDesc.Width;
	m_outputHeight = backBufferDesc.Height;
    

    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;
	
    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));
	backBuffer->Release();
    
    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));
    
    // TODO: Initialize windows-size dependent objects here.
    m_screenPos.x = backBufferWidth / 2.f;
	m_screenPos.y = backBufferHeight / 2.f;
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
	m_spriteBatch.reset();

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
   // m_swapChain.Reset();
   // m_d3dContext.Reset();
   // m_d3dDevice.Reset();

    m_texture.Reset();

   // UpdateDevice();

    //UpdateResources();
}