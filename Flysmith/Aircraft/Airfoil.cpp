#include "PCH.h"
#include "Airfoil.h"
#include "Resources\AssetLocator.h"
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
	AssetLocator assLocator;
	std::wstring path;
	if (!assLocator.GetAssetPath(AssetType::AIRFOILS, filename, &path))
	{
		MessageBoxA(NULL, "The airfoil file with the provided name doesn't exist. There's no error \"handling\" and this right here is in the constr,\
							because I'm a moron. Close the application.", NULL, MB_OK);
	}

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
		mesh.verts.push_back(foilPoint);
	}

	auto numPoints = points.size();
	for (U32 i = 1; i < numPoints; i++)
	{
		mesh.indices.push_back(i);
		mesh.indices.push_back(numPoints - i);
		mesh.indices.push_back(i - 1);
	}

	mesh.GenerateNormals();

	return mesh;
}
