#include "PCH.h"
#include "SceneGraph.h"


SceneNode* SceneGraph::AddNode(I32 parentNodeId)
{
	SceneNode newNode;
	newNode.id = nodes.size();
	newNode.parent = parentNodeId;
	newNode.pSceneGraph = this;

	nodes.push_back(newNode);
	return &nodes[newNode.id];
}