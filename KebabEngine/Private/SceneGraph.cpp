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
	newNode.pSceneGraph = this;
	newNode.parent = parentNodeId;
	
	if (parentNodeId != -1)
	{
		nodes[parentNodeId].children.push_back(newNode.id);
	}

	nodes[numNodes++] = newNode;
	return &nodes[newNode.id];
}