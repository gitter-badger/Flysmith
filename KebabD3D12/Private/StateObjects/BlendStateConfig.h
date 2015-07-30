#pragma once
#include "PCH.h"


namespace cuc
{
	struct BlendStateConfig : public D3D12_BLEND_DESC
	{
		BlendStateConfig();
	};

	const BlendStateConfig kDefaultBlendState;
}