#include "PCH.h"
#include "SceneGraph.h"


SceneGraph::SceneGraph()
	: numNodes(0)
{
}

bool SceneGraph::FormsCycle(I32 child, I32 parent)
{
	assert(child >= 0);
	assert(parent >= 0);

	I32 tempNode = child;
	while (tempNode != -1)
	{
		if (tempNode == parent)
		{
			return true;
		}

		tempNode = nodes[tempNode].parent;
	}

	return false;
}

SceneNode* SceneGraph::AddNode(I32 parentNodeId)
{
	SceneNode newNode;
	newNode.pSceneGraph = this;
	
	newNode.id = numNodes;
	assert(newNode.id != parentNodeId);
	newNode.parent = parentNodeId;

	if (parentNodeId != -1)
	{
		nodes[parentNodeId].children.push_back(newNode.id);
	}

	nodes[numNodes++] = newNode;
	return &nodes[newNode.id];
}