#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>


namespace cuc
{
	struct RasterizerStateConfig : public D3D12_RASTERIZER_DESC
	{
		RasterizerStateConfig(const D3D12_FILL_MODE fillMode = D3D12_FILL_MODE_SOLID,
							  const D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK);
	};
}

#endif