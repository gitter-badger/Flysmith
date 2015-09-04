#include "PCH.h"
#include "Scene.h"


Scene::Scene()
{
	numEntities = 0;
	for (size_t entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
		entities[entityIndex].m_id = entityIndex;
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
	}

	return ComponentProxy(type, index);
}

void Scene::Init(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

ComponentProxy Scene::CreateRenderComponent(U32 mesh, U32 vertShader, U32 pixelShader)
{
	renderComponents.push_back(RenderComponent(m_pRenderer, mesh, vertShader, pixelShader));
	return ComponentProxy(Component::Type::RENDER, renderComponents.size() - 1);
}

U32 Scene::CreateEntity(Transform transform, I32 parentEntity)
{
	auto newEntityIndex = numEntities;
	numEntities++;

	entities[newEntityIndex].pSceneNode = sceneGraph.AddNode(parentEntity);
	entities[newEntityIndex].SetTransform(transform);

	return newEntityIndex;
}