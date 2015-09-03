#include "PCH.h"
#include "SceneGraph.h"


SceneGraph::SceneGraph()
	: numNodes(0)
{
}

SceneNode* SceneGraph::AddNode(I32 parentNodeId)
{
	SceneNode newNode;
	newNode.id = numNodes;
	newNode.parent = parentNodeId;
	newNode.pSceneGraph = this;

	nodes[numNodes++] = newNode;
	return &nodes[newNode.id];
}