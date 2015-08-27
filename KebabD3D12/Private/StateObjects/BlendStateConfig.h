#pragma once
#include "PCH.h"


struct BlendStateConfig : public D3D12_BLEND_DESC
{
	BlendStateConfig();
};

const BlendStateConfig kDefaultBlendState;