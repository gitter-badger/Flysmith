#pragma once
#include "PCH.h"


struct RasterizerStateConfig : public D3D12_RASTERIZER_DESC
{
	RasterizerStateConfig(const D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID,
		const D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK);
};

const RasterizerStateConfig kDefaultRasterizerState;