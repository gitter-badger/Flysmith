#include "PCH.h"
#include "Fuselage.h"
#include "StandardUnits.h"


// Format:
// num rings
// points per ring
// for each ring:
// x-axis displacement from front 
Mesh Fuselage::GenerateMesh(const std::wstring& path)
{
	U32 numRings = 0;
	U32 pointsPerRing = 0;
	std::vector<XMFLOAT2> ringDisp;

	std::wifstream file(path);
	file >> numRings;
	file >> pointsPerRing;

	ringDisp.resize(numRings);

	// Paper measurements
	for (U32 ringIndex = 0; ringIndex < numRings; ringIndex++)
	{
		file >> ringDisp[ringIndex].x
			 >> ringDisp[ringIndex].y;
	}

	F32 ringDiameters[] = {
		1.0f, 2.0f, 3.5f, 4.0f, 4.5f, 3.5f, 1.f
	};

	// Real length: 8.28m
	// Paper length: 0.29m
	auto paperToReal = 8.28f / 0.29f * 0.01; // ringDist and ringDiam are in cm 

	for (auto& disp : ringDisp)
	{
		disp.x *= paperToReal;
		disp.y *= paperToReal;
	}

	for (auto& diameter : ringDiameters)
	{
		diameter *= paperToReal;
	}


	std::vector<std::vector<Vertex>> rings(numRings);
	// Generate vertex data
	for (U32 idx = 0; idx < numRings; idx++)
	{
		rings[idx] = GenerateCircularRing(MetersToDXUnits(ringDiameters[idx]), { 0.0f, MetersToDXUnits(ringDisp[idx].y), MetersToDXUnits(ringDisp[idx].x) });
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