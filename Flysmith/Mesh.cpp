#include "PCH.h"
#include "Mesh.h"


void Mesh::SetQuadFace(U32 v0, U32 v1, U32 v2, U32 v3)
{
	indices.push_back(v0);
	indices.push_back(v1);
	indices.push_back(v3);

	indices.push_back(v3);
	indices.push_back(v1);
	indices.push_back(v2);
}

void Mesh::GenerateNormals()
{
	U32 numTriangles = indices.size() / 3;
	for (U32 i = 0; i < numTriangles; ++i)
	{
		U32 i0 = indices[i * 3 + 0],
			i1 = indices[i * 3 + 1],
			i2 = indices[i * 3 + 2];

		auto v0 = verts[i0].position.GetXMVec(),
			 v1 = verts[i1].position.GetXMVec(),
			 v2 = verts[i2].position.GetXMVec();

		auto faceNormal = XMVector3Cross((v1 - v0), (v2 - v0));

		auto n0 = verts[i0].normal.GetXMVec(),
			 n1 = verts[i1].normal.GetXMVec(),
			 n2 = verts[i2].normal.GetXMVec();

		verts[i0].normal.Set(n0 + faceNormal);
		verts[i1].normal.Set(n1 + faceNormal);
		verts[i2].normal.Set(n2 + faceNormal);
	}

	for (auto& vert : verts)
	{
		vert.normal.Set(XMVector3Normalize(vert.normal.GetXMVec()));
	}
}

void Mesh::StitchRings(U32 numVertsInRing, U32 firstRingStart, U32 secondRingStart)
{
	for (U32 vertOffset = 0; vertOffset < numVertsInRing - 1; vertOffset++)
	{
		SetQuadFace(firstRingStart  + vertOffset,
					secondRingStart + vertOffset,
					secondRingStart + vertOffset + 1,
					firstRingStart  + vertOffset + 1);
	}

	// Quad delimited by start edge and end edge
	/*SetQuadFace(secondRingStart + numVertsInRing - 1,
				secondRingStart,
				firstRingStart,
				firstRingStart + numVertsInRing - 1);*/

	SetQuadFace(secondRingStart,
				firstRingStart,
				firstRingStart + numVertsInRing - 1,
				secondRingStart + numVertsInRing - 1);
}