#pragma once
#include "Resources\ResourceCache.h"
#include "Transform.h"


namespace cuc
{
	struct RenderObject
	{
		Transform transform;
		ResourceHandle meshHandle;
		ResourceHandle vertShaderHandle;
		ResourceHandle pixelShaderHandle;
	};
}