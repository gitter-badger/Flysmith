#include "PCH.h"
#include "Scene.h"


const U32 Scene::s_sceneKey = 1337123456789;

Scene::Scene()
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

	I32 parentEntityNodeId = -1;
	if (parentEntity != -1)
	{
		parentEntityNodeId = entities[parentEntity].GetSceneNodeId();
	}

	entities[newEntityIndex].Init(s_sceneKey, newEntityIndex, sceneGraph.AddNode(parentEntityNodeId));
	entities[newEntityIndex].SetTransform(transform);

	return newEntityIndex;
}