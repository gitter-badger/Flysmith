#pragma once
#include "PCH.h"
#include "../KebabD3D12/Public/Vertex.h"
using namespace DirectX;


struct Mesh
{
	std::vector<U32> indices;
	std::vector<Vertex> verts;

	void GenerateNormals()
	{
		U32 numTriangles = indices.size() / 3;
		for (U32 i = 0; i < numTriangles; ++i)
		{
			U32 i0 = indices[i * 3 + 0],
				i1 = indices[i * 3 + 1],
				i2 = indices[i * 3 + 2];

			auto v0 = XMLoadFloat3(&verts[i0].position),
				 v1 = XMLoadFloat3(&verts[i1].position),
				 v2 = XMLoadFloat3(&verts[i2].position);

			auto faceNormal = XMVector3Cross((v1 - v0), (v2 - v0));

			auto n0 = XMLoadFloat3(&verts[i0].normal),
				 n1 = XMLoadFloat3(&verts[i1].normal),
				 n2 = XMLoadFloat3(&verts[i2].normal);

			XMStoreFloat3(&verts[i0].normal, n0 + faceNormal);
			XMStoreFloat3(&verts[i1].normal, n1 + faceNormal);
			XMStoreFloat3(&verts[i2].normal, n2 + faceNormal);
		}

		for (auto& vert : verts)
		{
			XMStoreFloat3(&vert.normal, XMVector3Normalize(XMLoadFloat3(&vert.normal)));
		}
	}
};