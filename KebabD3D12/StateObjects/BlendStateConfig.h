#pragma once
#include "PCH.h"


namespace cuc
{
	struct KEBAB12_API BlendStateConfig : public D3D12_BLEND_DESC
	{
		BlendStateConfig();
	};

	const BlendStateConfig kDefaultBlendState;
}