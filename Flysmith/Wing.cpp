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

	std::sort(rings.begin(), rings.end(), [](const WingRing& lhs, const WingRing& rhs) -> bool { 
		return lhs.locationOnWing < rhs.locationOnWing; 
	});
	


	std::vector<Airfoil> airfoils;
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		airfoils.push_back(Airfoil(airfoilFile + L".dat"));
	}

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		XMFLOAT3 scalingVec = { rings[ringIdx].chord, rings[ringIdx].chord, 1.0f };
		auto scalingMat = XMMatrixScalingFromVector(XMLoadFloat3(&scalingVec));

		auto rotMat = XMMatrixRotationZ(XMConvertToRadians(rings[ringIdx].incidenceAngle));

		for (auto& point : airfoils[ringIdx].points)
		{
			XMStoreFloat2(&point, XMVector2Transform(XMLoadFloat2(&point), scalingMat));
			XMStoreFloat2(&point, XMVector2Transform(XMLoadFloat2(&point), rotMat));
		}
	}

	Mesh mesh;

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoils[ringIdx].points)
		{
			XMFLOAT3 pos(point.x, point.y, length * rings[ringIdx].locationOnWing);
			XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
			mesh.verts.push_back({ pos, normal });
		}
	}

	auto ringSize = airfoils[0].points.size();
	for (U32 ringIdx = 0; ringIdx < rings.size() - 1; ringIdx++)
	{
		mesh.StitchRings(ringSize, ringSize * ringIdx,
								   ringSize * (ringIdx + 1));
	}

	mesh.GenerateNormals();

	return mesh;
}