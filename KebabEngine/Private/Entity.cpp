#include "PCH.h"
#include "Entity.h"
#include "Scene.h"


Entity::Entity()
{
}

I32 Entity::GetId()
{
	return m_id;
}

XMFLOAT4X4 Entity::GetWorldTransform()
{
	return pSceneNode->GetWorldTransform();
}

void Entity::AttachComponent(const ComponentProxy& componentProxy, Component* pComponent)
{
	pComponent->AttachToEntity(m_id);
	components.push_back(componentProxy);
}

void Entity::AttachComponent(Component::Type type, U32 componentIndex, Component* pComponent)
{
	pComponent->AttachToEntity(m_id);
	components.push_back(ComponentProxy(type, componentIndex));
}