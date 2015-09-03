#include "PCH.h"
#include "SceneNode.h"
#include "SceneGraph.h"


XMFLOAT4X4 SceneNode::GetWorldTransform()
{
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, GetWorldTransformXM());
	return world;
}

XMMATRIX SceneNode::GetWorldTransformXM()
{
	if (parent == -1)
	{
		return transform.GetMatrixXM();
	}

	return transform.GetMatrixXM() * pSceneGraph->nodes[parent].transform.GetMatrixXM();
}