#include "PCH.h"
#include "Scene.h"


const U32 Scene::s_sceneKey = 1337;

Scene::Scene()
	: m_pRenderer(nullptr)
{
	numEntities = 0;
}

ComponentProxy Scene::AddComponent(Component* pComponent)
{
	auto type = pComponent->GetType();
	U32 index = 0;

	switch (type)
	{
	case Component::RENDER:
		renderComponents.push_back(*reinterpret_cast<RenderComponent*>(pComponent));
		break;
	case Component::PHYSICS:
		physicsComponents.push_back(*reinterpret_cast<PhysicsComponent*>(pComponent));
		break;
	default:
		break;
	}

	return ComponentProxy(type, index);
}

void Scene::Init(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

ComponentProxy Scene::CreateRenderComponent(U32 meshHandle, U32 vertShaderHandle, U32 pixelShaderHandle)
{
	renderComponents.push_back(RenderComponent(m_pRenderer, meshHandle, vertShaderHandle, pixelShaderHandle));
	return ComponentProxy(Component::Type::RENDER, renderComponents.size() - 1);
}

ComponentProxy Scene::CreatePhysicsComponent()
{
	physicsComponents.push_back(PhysicsComponent());
	return ComponentProxy(Component::Type::PHYSICS, physicsComponents.size() - 1);
}

void Scene::AttachComponent(EntityId entityId, const ComponentProxy& component)
{
	entities[entityId].AttachComponent(component, &renderComponents[component.index]);
}

EntityId Scene::CreateEntity(Transform transform, I32 parentEntity)
{
	auto newEntityIndex = numEntities;
	numEntities++;

	auto parentEntityNodeId = -1;
	if (parentEntity != -1)
	{
		parentEntityNodeId = entities[parentEntity].GetSceneNodeId();
	}

	entities[newEntityIndex].Init(s_sceneKey, newEntityIndex, sceneGraph.AddNode(parentEntityNodeId));
	entities[newEntityIndex].SetTransform(transform);

	return newEntityIndex;
}
