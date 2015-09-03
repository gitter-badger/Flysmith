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

void Entity::AttachComponent(Component* pNewComponent)
{
	pNewComponent->AttachToEntity(m_id);
}