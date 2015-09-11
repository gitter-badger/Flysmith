#include "PCH.h"
#include "Wing.h"
#include "Mesh.h"
#include "Airfoil.h"


WingRing::WingRing()
	: locationOnWing(0.0f)
	, chord(0.0f)
	, incidenceAngle(0.0f)
{
}

Wing::Wing()
	: length(0.0f)
{
}

Mesh Wing::GenerateMesh()
{
	assert(length > 0.0f);
	assert(airfoilFile.size());
	assert(rings.size() >= 2);
	
	for (U32 ringIdx = 1; ringIdx < rings.size(); ringIdx++)
		assert(rings[ringIdx].locationOnWing != rings[ringIdx - 1].locationOnWing);
	
	for (auto& ring : rings)
		assert(ring.chord > 0.0f);

	// Sort rings by location on wing 
	std::sort(rings.begin(), rings.end(), [](const WingRing& lhs, const WingRing& rhs) -> bool { 
		return lhs.locationOnWing < rhs.locationOnWing; 
	});
	
	// Generate section ends 
	std::vector<std::vector<XMFLOAT3>> airfoils(rings.size());
	Airfoil airfoil(airfoilFile + L".dat");
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for(auto& point : airfoil.points)
			airfoils[ringIdx].push_back({ point.x, point.y, 0.0f });
	}

	// Scale by chord
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		XMFLOAT3 scalingVec = { rings[ringIdx].chord, rings[ringIdx].chord, 1.0f };
		auto scalingMat = XMMatrixScalingFromVector(XMLoadFloat3(&scalingVec));

		auto rotMat = XMMatrixRotationZ(XMConvertToRadians(rings[ringIdx].incidenceAngle));

		for (auto& point : airfoils[ringIdx])
		{
			XMStoreFloat3(&point, XMVector3Transform(XMLoadFloat3(&point), scalingMat));
			XMStoreFloat3(&point, XMVector3Transform(XMLoadFloat3(&point), rotMat));
		}
	}

	// Place rings on wing 
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoils[ringIdx])
			point.z = length * rings[ringIdx].locationOnWing;
	}

	// Sweep
	for (U32 sectionIdx = 0; sectionIdx < sections.size(); ++sectionIdx)
	{
		auto sweepAngle = XMConvertToRadians(sections[sectionIdx].sweepAngle);
		for (auto& point : airfoils[sectionIdx + 1])
		{
			point.x = point.x * cos(sweepAngle) + point.z * sin(sweepAngle);
		}
	}

	// Generate mesh
	Mesh mesh;

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);

		for (auto& point : airfoils[ringIdx])
		{
			mesh.verts.push_back({ point, normal });
		}
	}

	auto ringSize = airfoils[0].size();
	for (U32 ringIdx = 0; ringIdx < rings.size() - 1; ringIdx++)
	{
		mesh.StitchRings(ringSize, ringSize * ringIdx,
								   ringSize * (ringIdx + 1));
	}

	mesh.GenerateNormals();

	return mesh;
}