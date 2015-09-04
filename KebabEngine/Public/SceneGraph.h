#pragma once
#include "PublicDef.h"
#include "SceneNode.h"
#include <vector>


const size_t MAX_NODES = 100;

// Used only for transform hierarchization.
// For now, no transform caching is going on inside the scene graph object(the outside will likely do it).
struct KEBAB_API SceneGraph
{
	SceneGraph();
	bool FormsCycle(U32 child, U32 parent);

	// Returns a pointer to the new node
	SceneNode* AddNode(I32 parentNodeId = -1);
	SceneNode nodes[MAX_NODES];
	size_t numNodes;
};