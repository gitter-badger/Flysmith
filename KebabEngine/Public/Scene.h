#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "RenderComponent.h"
#include "SceneGraph.h"
#include "Entity.h"
#include <vector>


class KEBAB_API Scene
{
public:
	TransformNoScale camTransform;
	std::vector<Entity> entities;
	std::vector<RenderComponent> renderComponents;
	SceneGraph sceneGraph;
};