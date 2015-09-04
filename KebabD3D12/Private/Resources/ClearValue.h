#pragma once
#include "PCH.h"


struct ClearValue : D3D12_CLEAR_VALUE
{
	ClearValue(DXGI_FORMAT, F32 color[4]);
	ClearValue(DXGI_FORMAT, F32 depth = 0.0f, U8 stencil = 0);
};