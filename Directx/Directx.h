#pragma once

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#include "../SDK/CClientInstance.h"
#include "../SDK/CGameMode.h"
#include "../Utils/TextFormat.h"


bool GetD3D11SwapchainDeviceContext(void ** pSwapchainTable, size_t Size_Swapchain, void ** pDeviceTable, size_t Size_Device, void ** pContextTable, size_t Size_Context);
void* getSwapChain();
void Draw();

#ifndef D3DXFLOAT16
typedef struct D3DXFLOAT16
{
public:
	D3DXFLOAT16() {};
	D3DXFLOAT16(FLOAT);
	D3DXFLOAT16(CONST D3DXFLOAT16&);

	// casting
	operator FLOAT ();

	// binary operators
	BOOL operator == (CONST D3DXFLOAT16&) const;
	BOOL operator != (CONST D3DXFLOAT16&) const;

protected:
	WORD value;
} D3DXFLOAT16, *LPD3DXFLOAT16;
#endif


#ifndef _D3DVECTOR
typedef struct _D3DVECTOR {
	float x;
	float y;
	float z;
} D3DVECTOR;
#endif

#ifndef D3DXVECTOR3
typedef struct D3DXVECTOR3 : public D3DVECTOR
{
public:
	D3DXVECTOR3() {};
	D3DXVECTOR3(CONST FLOAT *);
	D3DXVECTOR3(CONST D3DVECTOR&);
	D3DXVECTOR3(CONST D3DXFLOAT16 *);
	D3DXVECTOR3(FLOAT x, FLOAT y, FLOAT z);

	// casting
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXVECTOR3& operator += (CONST D3DXVECTOR3&);
	D3DXVECTOR3& operator -= (CONST D3DXVECTOR3&);
	D3DXVECTOR3& operator *= (FLOAT);
	D3DXVECTOR3& operator /= (FLOAT);

	// unary operators
	D3DXVECTOR3 operator + () const;
	D3DXVECTOR3 operator - () const;

	// binary operators
	D3DXVECTOR3 operator + (CONST D3DXVECTOR3&) const;
	D3DXVECTOR3 operator - (CONST D3DXVECTOR3&) const;
	D3DXVECTOR3 operator * (FLOAT) const;
	D3DXVECTOR3 operator / (FLOAT) const;

	friend D3DXVECTOR3 operator * (FLOAT, CONST struct D3DXVECTOR3&);

	BOOL operator == (CONST D3DXVECTOR3&) const;
	BOOL operator != (CONST D3DXVECTOR3&) const;

} D3DXVECTOR3, *LPD3DXVECTOR3;
#endif

#ifndef D3DXCOLOR
typedef struct D3DXCOLOR
{
public:
	D3DXCOLOR() {};
	D3DXCOLOR(UINT  argb);
	D3DXCOLOR(CONST FLOAT *);
	D3DXCOLOR(CONST D3DXFLOAT16 *);
	D3DXCOLOR(FLOAT r, FLOAT g, FLOAT b, FLOAT a);

	// casting
	operator UINT  () const;

	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	D3DXCOLOR& operator += (CONST D3DXCOLOR&);
	D3DXCOLOR& operator -= (CONST D3DXCOLOR&);
	D3DXCOLOR& operator *= (FLOAT);
	D3DXCOLOR& operator /= (FLOAT);

	// unary operators
	D3DXCOLOR operator + () const;
	D3DXCOLOR operator - () const;

	// binary operators
	D3DXCOLOR operator + (CONST D3DXCOLOR&) const;
	D3DXCOLOR operator - (CONST D3DXCOLOR&) const;
	D3DXCOLOR operator * (FLOAT) const;
	D3DXCOLOR operator / (FLOAT) const;

	friend D3DXCOLOR operator * (FLOAT, CONST D3DXCOLOR&);

	BOOL operator == (CONST D3DXCOLOR&) const;
	BOOL operator != (CONST D3DXCOLOR&) const;

	FLOAT r, g, b, a;
} D3DXCOLOR, *LPD3DXCOLOR;
#endif

struct COLOR_VERTEX
{
	D3DXVECTOR3 Position;
	D3DXCOLOR    Color;
};

