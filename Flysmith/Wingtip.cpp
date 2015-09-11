#include "PCH.h"
#include "Wingtip.h"
#include "Mesh.h"
#include "Airfoil.h"


Wingtip::Wingtip()
	: chord(0.0f)
{
}

Mesh Wingtip::GenerateMesh()
{
	assert(chord > 0.0f);
	assert(airfoilFile.size());

	Airfoil airfoil(airfoilFile + L".dat");
	std::vector<XMFLOAT3> points;
	for (auto& point : airfoil.points)
	{
		points.push_back({ point.x, point.y, 0.0f });
	}

	auto scalingMat = XMMatrixScaling(chord, chord, 1.0f);

	for (auto& point : points)
	{
		XMStoreFloat3(&point, XMVector3Transform(XMLoadFloat3(&point), scalingMat));
	}

	Mesh mesh;
	
	switch (type)
	{
	case CUTOFF:
		GenerateCutoff(mesh, points);
		break;
	}

	mesh.GenerateNormals();
	
	return mesh;
}

void Wingtip::GenerateCutoff(Mesh& mesh, std::vector<XMFLOAT3>& airfoil)
{
	XMFLOAT3 normalVec = { 0.0f, 0.0f, 0.0f };

	// No need to generate additional verts
	for (auto& point : airfoil)
	{
		mesh.verts.push_back({ point, normalVec });
	}
		
	auto numPoints = airfoil.size();
	for (U32 pointIdx = 0; pointIdx < numPoints - 1; ++pointIdx)
	{
		mesh.indices.push_back(pointIdx);
		mesh.indices.push_back(pointIdx + 1);
		mesh.indices.push_back(numPoints - 1 - pointIdx);
	}
}