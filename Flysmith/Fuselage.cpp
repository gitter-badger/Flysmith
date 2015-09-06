#include "PCH.h"
#include "Fuselage.h"
#include "StandardUnits.h"


Mesh Fuselage::GenerateMesh()
{
	// Paper measurements
	F32 ringDist[] = {
		0.0f, 0.5f, 2.0f, 5.0f, 14.0f, 16.0f, 29.0f
	};

	F32 ringDiameters[] = {
		1.0f, 2.0f, 3.5f, 4.0f, 4.5f, 3.5f, 0.1f
	};

	// Real length: 8.28m
	// Paper length: 0.29m
	auto paperToReal = 8.28f / 0.29f * 0.01; // ringDist and ringDiam are in cm 

	for (auto& distance : ringDist)
	{
		distance *= paperToReal;
	}

	for (auto& diameter : ringDiameters)
	{
		diameter *= paperToReal;
	}

	auto numRings = _countof(ringDist);

	std::vector<std::vector<Vertex>> rings(numRings);
	// Generate vertex data
	for (U32 idx = 0; idx < numRings; idx++)
	{
		rings[idx] = GenerateCircularRing(MetersToDXUnits(ringDiameters[idx]), { 0.0f, 0.0f, MetersToDXUnits(ringDist[idx]) });
	}

	Mesh mesh;
	for (auto& ring : rings)
		mesh.verts.insert(mesh.verts.end(), ring.begin(), ring.end());
	
	// *Stable* sort by z coord

	// Stitch
	auto ringSize = mesh.verts.size() / numRings;
	for (U32 ringIndex = 0; ringIndex < numRings - 1; ringIndex++)
	{
		mesh.StitchRings(ringSize, ringSize * ringIndex,
								   ringSize * (ringIndex + 1));
	}

	mesh.GenerateNormals();

	return mesh;
}

// TODO: Change to displacement from center
std::vector<Vertex> Fuselage::GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront)
{
	F32 radius = diameter / 2.0f;

	std::vector<Vertex> verts;

	for (F32 angle = 0; angle <= XM_2PI; angle += 0.1f)
	{
		XMFLOAT3 pos(radius * cos(angle),
			radius * sin(angle) + dispFromFront.y,
			dispFromFront.z);
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
		verts.push_back({ pos, normal });
	}

	return verts;
}