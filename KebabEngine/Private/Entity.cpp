#include "PCH.h"
#include "Entity.h"
#include "Scene.h"


I32 Entity::s_nextId = 0;

Entity::Entity()
{
	m_id = s_nextId;
	s_nextId++;
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