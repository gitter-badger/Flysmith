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

void Entity::AttachComponent(U32 componentIndex, Component* pComponent)
{
	pComponent->AttachToEntity(m_id);
	components.push_back(ComponentProxy(pComponent->GetType(), componentIndex));
}

void Entity::SetParent(Entity* pParent)
{
	pSceneNode->parent = pParent->pSceneNode->parent;
	pParent->pSceneNode->children.push_back(pSceneNode->id);
}

void Entity::AddChild(Entity* pChild)
{
	pSceneNode->children.push_back(pChild->pSceneNode->id);
	pChild->pSceneNode->parent = pSceneNode->id;
}