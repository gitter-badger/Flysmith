#include "PCH.h"
#include "Wing.h"


// TODO: Wing tip generation 
// TODO: Control surfaces(separate?)
// TODO: Sweep
// TODO: Variable thickness 
// TODO: Variable airfoil
// TODO: Innards 
Mesh Wing::GenerateMesh()
{
	// Measured from one wing tip to the other(i.e. includes main body)
	F32 wingspan = MetersToDXUnits(7.0f);
	F32 bodyDiameter = MetersToDXUnits(2.0f);
	F32 length = (wingspan - bodyDiameter) / 2.0f;
	
	std::vector<Airfoil> crossSections;
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		crossSections.push_back(Airfoil(airfoilFile + L".dat"));
	}

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		F32 scaledChord = MetersToDXUnits(rings[ringIdx].chord);
		XMFLOAT3 scalingVec = { scaledChord, scaledChord, 1.0f };
		auto scalingMat = XMMatrixScalingFromVector(XMLoadFloat3(&scalingVec));

		for (auto& point : crossSections[ringIdx].points)
		{
			XMStoreFloat2(&point, XMVector2Transform(XMLoadFloat2(&point), scalingMat));
		}
	}

	Mesh mesh;

	// 2. Generate vertices on rings 
	for (U32 ringIndex = 0; ringIndex < rings.size(); ringIndex++)
	{
		for (auto& point : crossSections[ringIndex].points)
		{
			XMFLOAT3 pos(point.x, point.y, length * rings[ringIndex].locationOnWing);
			XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
			mesh.verts.push_back({ pos, normal });
		}
	}

	// 3. Stitch
	auto ringSize = crossSections[0].points.size();
	for (U32 ringIndex = 0; ringIndex < rings.size() - 1; ringIndex++)
	{
		mesh.StitchRings(ringSize, ringSize * ringIndex,
								   ringSize * (ringIndex + 1));
	}

	mesh.GenerateNormals();

	return mesh;
}