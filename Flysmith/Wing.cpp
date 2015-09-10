#include "PCH.h"
#include "Wing.h"
#include "Mesh.h"
#include "Airfoil.h"
#include "StandardUnits.h"


Mesh Wing::GenerateMesh()
{
	// Measured from one wing tip to the other(i.e. includes main body)
	F32 wingspan = MetersToDXUnits(7.0f);
	F32 bodyDiameter = MetersToDXUnits(2.0f);
	F32 length = (wingspan - bodyDiameter) / 2.0f;
	
	std::vector<Airfoil> airfoils;
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		airfoils.push_back(Airfoil(airfoilFile + L".dat"));
	}

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		F32 scaledChord = MetersToDXUnits(rings[ringIdx].chord);
		XMFLOAT3 scalingVec = { scaledChord, scaledChord, 1.0f };
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