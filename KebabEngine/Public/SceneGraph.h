#pragma once
#include "PublicDef.h"
#include "SceneNode.h"
#include <vector>


// Used only for transform hierarchization.
// For now, no transform caching is going on inside the scene graph object(the outside will likely do it).
struct KEBAB_API SceneGraph
{
	// Returns a pointer to the new node
	SceneNode* AddNode(I32 parentNodeId = -1);
	
	std::vector<SceneNode> nodes;
};