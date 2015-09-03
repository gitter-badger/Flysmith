#pragma once
#include "PublicDef.h"
#include "Component.h"
#include "SceneNode.h"
#include <vector>


class KEBAB_API Entity
{
public:
	// TODO: A component proxy that indicated a component's type and component id.
	//       Store only component proxies here.
	//       Each component is then stored in its type's container.
	std::vector<Component> m_components;
	Entity();
	I32 GetId();

	XMFLOAT4X4 GetWorldTransform();
	SceneNode sceneNode;

protected:
	I32 m_id;

private:
	static I32 s_nextId;
};