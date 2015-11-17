#pragma once
#include "PublicDef.h"
#include "Component.h"
#include "SceneNode.h"
#include "ComponentProxy.h"
#include <vector>


using EntityId = I32;

class KEBAB_API Entity
{
public:
	Entity();
	void Init(U32 sceneKey, I32 id, SceneNode*);

	I32 GetId() const;
	I32 GetSceneNodeId() const;

	void AttachComponent(const ComponentProxy&, Component*);
	void AttachComponent(U32 componentIndex, Component*);

	void SetParent(Entity*);
	void AddChild(Entity*);

	void SetTransform(const Transform&);
	Transform* GetTransform() const;
	XMFLOAT4X4 GetWorldTransformMatrix() const;

	std::vector<ComponentProxy> components;

protected:
	EntityId m_id;
	SceneNode* m_pSceneNode;

private:
	bool m_bInitialized;

	// Shared between the scene and entities so intialization is done once only by the scene.
	// The scene then no longer has direct access to the entity's private state beyond initialization.
	static const U32 s_sceneKey;
};