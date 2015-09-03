#include "PCH.h"
#include "SceneNode.h"
#include "SceneGraph.h"


XMFLOAT4X4 SceneNode::GetWorldTransform(SceneGraph* pSceneGraph)
{
	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, GetWorldTransformXM(pSceneGraph));
	return world;
}

XMMATRIX SceneNode::GetWorldTransformXM(SceneGraph* pSceneGraph)
{
	return transform.GetMatrixXM() * pSceneGraph->nodes[parent].transform.GetMatrixXM();
}