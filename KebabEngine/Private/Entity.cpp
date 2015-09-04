#include "PCH.h"
#include "Entity.h"
#include "Scene.h"


const U32 Entity::s_sceneKey = 1337123456789;

Entity::Entity()
	: m_id(-1)
	, m_pSceneNode(nullptr)
	, m_bInitialized(false)
{
}

void Entity::Init(U32 sceneKey, I32 id, SceneNode* pSceneNode)
{
	assert(!m_bInitialized);
	assert(sceneKey == s_sceneKey);
	assert(pSceneNode != nullptr);
	assert(id >= 0);

	m_id = id;
	m_pSceneNode = pSceneNode;
	m_bInitialized = true;
}

U32 Entity::GetId()
{
	assert(m_bInitialized);
	return m_id;
}

U32 Entity::GetSceneNodeId()
{
	assert(m_bInitialized);
	return m_pSceneNode->id;
}

XMFLOAT4X4 Entity::GetWorldTransformMatrix()
{
	return m_pSceneNode->GetWorldTransform();
}

void Entity::SetTransform(const Transform& transform)
{
	m_pSceneNode->transform = transform;
}

Transform* Entity::GetTransform()
{
	return &m_pSceneNode->transform;
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
	// TODO: Update old relationship
	m_pSceneNode->parent = pParent->m_pSceneNode->parent;
	pParent->m_pSceneNode->children.push_back(m_pSceneNode->id);
}

void Entity::AddChild(Entity* pChild)
{
	// TODO: Update old relationship
	m_pSceneNode->children.push_back(pChild->m_pSceneNode->id);
	pChild->m_pSceneNode->parent = m_pSceneNode->id;
}