#include "PCH.h"
#include "Mesh.h"


void cuc::Mesh::AddVertex(float x, float y)
{
	verts.push_back({ { x, y, 0.0f },
					  { 1.0f, 0.0f, 0.0f, 1.0f }
					});
}

void cuc::Mesh::AddVertex(DirectX::XMFLOAT2 pos)
{
	verts.push_back({ { pos.x, pos.y, 0.0f },
					  { 1.0f, 0.0f, 0.0f, 1.0f }
					});
}

void cuc::Mesh::SetTriangle(U32 v0, U32 v1, U32 v2)
{
	indices.push_back(v0);
	indices.push_back(v1);
	indices.push_back(v2);
}
