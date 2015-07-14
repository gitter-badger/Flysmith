#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>


namespace cuc
{
	struct BlendStateConfig : public D3D12_BLEND_DESC
	{
		BlendStateConfig();
	};

	const BlendStateConfig kDefaultBlendState;
}

#endif