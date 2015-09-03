#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "RenderComponent.h"
#include "SceneGraph.h"
#include <vector>


class KEBAB_API Scene
{
public:
	TransformNoScale m_camTransform;
	// TEMP
	std::vector<RenderComponent> m_renderComponents;

	SceneGraph sceneGraph;
};