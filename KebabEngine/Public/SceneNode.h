#pragma once
#include "../../KebabCommon/Transform.h"
#include <vector>
#include "PublicDef.h"


struct SceneGraph;

struct KEBAB_API SceneNode
{
	// Relative to parent
	Transform transform;
	
	// Indices in the associated scene graph's node list. 
	I32 id;
	I32 parent;
	std::vector<I32> children;
	SceneGraph* pSceneGraph;

	XMFLOAT4X4 GetWorldTransform();
	XMMATRIX GetWorldTransformXM();
};