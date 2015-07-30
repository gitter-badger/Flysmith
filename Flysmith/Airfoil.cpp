#include "PCH.h"
#include "Airfoil.h"
#include "AssetLocator.h"


// Point order: 
//                _________________
//               /                 \
// leading edge 2                   1   trailing edge
//              3                   4
//               \_________________/
//


bool Airfoil::LoadFromFile(const wchar_t* filename)
{
	AssetLocator al;
	std::wstring path;
	if (!al.GetAssetPath(AssetDirectory::AIRFOILS, filename, &path))
	{
		return false;
	}

	std::ifstream file(path);
	std::string desc;
	file >> desc;

	float x, y;
	while ((file >> x) && (file >> y))
	{
		points.push_back({ x, y });
	}

	return true;
}
