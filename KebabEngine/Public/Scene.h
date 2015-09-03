#pragma once
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "RenderComponent.h"
#include "SceneGraph.h"
#include "Entity.h"
#include <vector>


const size_t MAX_ENTITIES = 100;

class KEBAB_API Scene
{
public:
	Scene();

	ComponentProxy AddComponent(Component*);

	TransformNoScale camTransform;
	Entity entities[MAX_ENTITIES];
	std::vector<RenderComponent> renderComponents;
	SceneGraph sceneGraph;
};