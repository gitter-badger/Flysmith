#include "PCH.h"
#include "Airfoil.h"
#include "AssetLocator.h"
using namespace DirectX;


// Point order: 
//                _________________
//               /                 \
// leading edge 2                   1   trailing edge
//              3                   4
//               \_________________/
//


Airfoil::Airfoil(const std::wstring& filename)
{
	AssetLocator al;
	std::wstring path;
	assert(al.GetAssetPath(AssetDirectory::AIRFOILS, filename, &path) == true);

	std::ifstream file(path);
	std::string desc;
	std::getline(file, desc);

	float x, y;
	while ((file >> x) && (file >> y))
	{
		// Center at (0, 0)
		points.push_back({ x - 0.5f, y - 0.5f });
	}
}

Mesh Airfoil::GenerateMesh()
{
	Mesh mesh;

	for (auto& foilPoint : points)
	{
		XMFLOAT2 vert(foilPoint);
		mesh.verts.push_back({ { vert.x, vert.y, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } });
	}

	auto numPoints = points.size();
	for (U32 i = 1; i < numPoints; i++)
	{
		mesh.indices.push_back(i);
		mesh.indices.push_back(numPoints - i);
		mesh.indices.push_back(i - 1);
	}

	return mesh;
}
