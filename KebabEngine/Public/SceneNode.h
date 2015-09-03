#pragma once
#include "../../KebabD3D12/Public/Transform.h"
#include <vector>
#include "PublicDef.h"


struct SceneGraph;

struct KEBAB_API SceneNode
{
	// Relative to parent
	Transform transform;
	
	// Indices in the associated scene graph's node list. 
	U32 id;
	I32 parent;
	std::vector<U32> children;
	SceneGraph* pSceneGraph;

	XMFLOAT4X4 GetWorldTransform();
	XMMATRIX GetWorldTransformXM();
};