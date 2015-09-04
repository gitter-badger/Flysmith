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
	void Init(Renderer*);

	ComponentProxy AddComponent(Component*);
	ComponentProxy CreateRenderComponent(U32 mesh, U32 vertShader, U32 pixelShader);

	// Returns the new entity's index
	// @parentEntity: leave -1 if the entity is not attached to any other
	// @relativeTransform: transform relative to parent(or directly world if the new entity does not have a parent)
	U32 CreateEntity(Transform transform = kDefaultTransform, I32 parentEntity = -1);

	TransformNoScale camTransform;
	
	std::vector<RenderComponent> renderComponents;
	
	SceneGraph sceneGraph;

	Entity entities[MAX_ENTITIES];
	size_t numEntities;

private:
	Renderer* m_pRenderer;

	// Shared between the scene and entities so intialization is done once only by the scene.
	// The scene then no longer has direct access to the entity's private state beyond initialization.
	static const U32 s_sceneKey;
};