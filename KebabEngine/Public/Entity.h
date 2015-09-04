#pragma once
#include "PublicDef.h"
#include "Component.h"
#include "SceneNode.h"
#include "ComponentProxy.h"
#include <vector>


class KEBAB_API Entity
{
public:
	Entity();
	void Init(U32 sceneKey, I32 id, SceneNode*);

	U32 GetId();
	U32 GetSceneNodeId();

	void AttachComponent(const ComponentProxy&, Component*);
	void AttachComponent(U32 componentIndex, Component*);

	void SetParent(Entity*);
	void AddChild(Entity*);

	void SetTransform(const Transform&);
	Transform* GetTransform();
	XMFLOAT4X4 GetWorldTransformMatrix();

	std::vector<ComponentProxy> components;

protected:
	I32 m_id;
	SceneNode* m_pSceneNode;

private:
	bool m_bInitialized;

	// Shared between the scene and entities so intialization is done once only by the scene.
	// The scene then no longer has direct access to the entity's private state beyond initialization.
	static const U32 s_sceneKey;
};