#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "../../KebabD3D12/Public/RenderComponent.h"
#include <vector>


namespace cuc
{
	class KEBAB_API Scene
	{
	public:
		TransformNoScale m_camTransform;
		// TEMP
		std::vector<RenderComponent> m_renderComponents;
	};
}