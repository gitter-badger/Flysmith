#include "PCH.h"
#include "Scene.h"


Scene::Scene()
{
	for (size_t entityIndex = 0; entityIndex < MAX_ENTITIES; entityIndex++)
	{
		entities[entityIndex].m_id = entityIndex;
	}
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