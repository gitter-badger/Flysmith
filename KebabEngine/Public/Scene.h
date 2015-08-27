#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "../../KebabD3D12/Public/RenderComponent.h"
#include <vector>


class KEBAB_API Scene
{
public:
	cuc::TransformNoScale m_camTransform;
	// TEMP
	std::vector<cuc::RenderComponent> m_renderComponents;
};