#include "PCH.h"
#include "Wing.h"


Wing::Wing(const std::wstring& airfoilFile) : m_airfoil(airfoilFile)
{
	GenerateMesh();
}

// TODO: Scale taking into account the chord
// TODO: Wing tip generation 
// TODO: Control surfaces(separate?)
// TODO: Sweep
// TODO: Variable thickness 
// TODO: Variable airfoil
// TODO: Innards 
void Wing::GenerateMesh()
{
	// Measured from one wing tip to the other(i.e. includes main body)
	F32 wingspan = MetersToDXUnits(7.0f);
	F32 bodyDiameter = MetersToDXUnits(2.0f);

	// 1. Generate ring configurations(for now all rings are the same).
	F32 wingWidth = (wingspan - bodyDiameter) / 2.0f;
	U32 numRings = 2;
	F32 ringOffsets[5] = { 0.0f, wingWidth };

	// 2. Generate vertices on rings 
	for (U32 ringIndex = 0; ringIndex < numRings; ringIndex++)
	{
		for (auto& point : m_airfoil.points)
		{
			XMFLOAT3 pos(point.x, point.y, ringOffsets[ringIndex]);
			XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
			m_mesh.verts.push_back({ pos, normal });
		}
	}

	// 3. Stitch
	auto ringSize = m_airfoil.points.size();
	for (U32 ringIndex = 0; ringIndex < numRings - 1; ringIndex++)
	{
		m_mesh.StitchRings(ringSize, ringSize * ringIndex,
									 ringSize * (ringIndex + 1));
	}

	m_mesh.GenerateNormals();
}

Mesh Wing::GetMesh()
{
	return m_mesh;
}