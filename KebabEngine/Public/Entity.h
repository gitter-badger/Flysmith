#pragma once
#include "PublicDef.h"
#include "Component.h"
#include "SceneNode.h"
#include "ComponentProxy.h"
#include <vector>


class KEBAB_API Entity
{
	friend class Scene;

public:
	Entity();

	I32 GetId();
	XMFLOAT4X4 GetWorldTransform();

	void AttachComponent(const ComponentProxy&, Component*);
	void AttachComponent(Component::Type, U32 componentIndex, Component*);

	SceneNode* pSceneNode;
	std::vector<ComponentProxy> components;

protected:
	I32 m_id;
};