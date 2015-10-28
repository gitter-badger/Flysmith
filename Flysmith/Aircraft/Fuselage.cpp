#include "PCH.h"
#include "Fuselage.h"
#include "Math\StandardUnits.h"
#include "Resources\AssetLocator.h"
#include "json.hpp"


void Fuselage::ReadFromFile(const std::wstring& filename)
{
	AssetLocator assLocator;
	std::wstring filePath;
	assLocator.GetAssetPath(AssetType::FUSELAGES, filename + L".json", &filePath);
	std::ifstream file(filePath);

	std::string data;
	std::string token;
	while (file >> token)
		data += token;

	auto config = nlohmann::json::parse(data);

	auto ringArray = config["rings"];
	for (size_t ringIdx = 0; ringIdx < ringArray.size(); ++ringIdx)
	{
		auto ringConfig = ringArray[ringIdx];

		FuselageRing ring;
		ring.diameter = ringConfig["diameter"];
		ring.x = ringConfig["x"];
		ring.y = ringConfig["y"];
		rings.push_back(ring);
	}
}

Mesh Fuselage::GenerateMesh()
{
	// TEMP
	// Real length: 8.28m
	// Paper length: 0.29m
	auto paperToReal = 8.28f / 0.29f * 0.01f; // ringDist and ringDiam are in cm 

	for (auto& ring : rings)
	{
		ring.x *= paperToReal;
		ring.y *= paperToReal;
		ring.diameter *= paperToReal;
	}

	Mesh mesh;
	for (auto& ring : rings)
	{
		auto ringVerts = GenerateCircularRing(MetersToDXUnits(ring.diameter), { 0.0f, MetersToDXUnits(ring.y), MetersToDXUnits(ring.x) });
		mesh.verts.insert(mesh.verts.end(), ringVerts.begin(), ringVerts.end());
	}

	// Stitch
	auto ringSize = mesh.verts.size() / rings.size();
	for (size_t ringIdx = 0; ringIdx < rings.size() - 1; ++ringIdx)
	{
		mesh.StitchRings(ringSize, ringSize * ringIdx,
								   ringSize * (ringIdx + 1));
	}

	mesh.GenerateNormals();

	return mesh;
}

// TODO: Change to displacement from center
std::vector<Vertex> Fuselage::GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront)
{
	F32 radius = diameter / 2.0f;

	std::vector<Vertex> verts;

	for (F32 angle = 0; angle <= XM_2PI; angle += 0.3f)
	{
		XMFLOAT3 pos(radius * cos(angle),
					 radius * sin(angle) + dispFromFront.y,
					 dispFromFront.z);
		verts.push_back(Vertex(pos));
	}

	return verts;
}