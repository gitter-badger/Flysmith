#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include <vector>


namespace cuc
{
	class KEBAB_API Scene
	{
	public:
		Transform camTransform;
		std::vector<Transform> objTransforms;
	};
}