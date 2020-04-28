//
// pch.h
// Header for standard system include files.
//

#pragma once

#include <winsdkver.h>
#include <sdkddkver.h>

// Use the C++ standard templated min/max
#define NOMINMAX

// DirectX apps don't need GDI
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP

// Include <mcx.h> if you need this
#define NOMCX

// Include <winsvc.h> if you need this
#define NOSERVICE

// WinHelp is deprecated
#define NOHELP

#define WIN32_LEAN_AND_MEAN
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11_1.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

#include <d3d11_3.h>
#include <dxgi1_4.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

namespace DX {
inline void ThrowIfFailed(HRESULT hr) {
	if (FAILED(hr)) {
		// Set a breakpoint on this line to catch DirectX API errors
		throw std::exception();
	}
}
}  // namespace DX


#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"