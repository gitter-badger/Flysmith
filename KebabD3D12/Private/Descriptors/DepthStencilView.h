#pragma once
#include "PCH.h"


struct DepthStencilView : D3D12_DEPTH_STENCIL_VIEW_DESC
{
	DepthStencilView(DXGI_FORMAT = DXGI_FORMAT_D32_FLOAT, D3D12_DSV_DIMENSION viewDimension = D3D12_DSV_DIMENSION_TEXTURE2D, D3D12_DSV_FLAGS = D3D12_DSV_FLAG_NONE);
	void Init(DXGI_FORMAT = DXGI_FORMAT_D32_FLOAT, D3D12_DSV_DIMENSION viewDimension = D3D12_DSV_DIMENSION_TEXTURE2D, D3D12_DSV_FLAGS = D3D12_DSV_FLAG_NONE);
};