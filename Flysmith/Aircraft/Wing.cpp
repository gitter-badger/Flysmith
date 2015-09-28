#include "PCH.h"
#include "Wing.h"
#include "Mesh.h"
#include "Airfoil.h"
#include "Math\AngleMath.h"


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

WingSection::WingSection()
	: sweep(0.0f)
	, dihedral(0.0f)
{
}

void Wing::CheckConfigurationValidity()
{
	assert(length > 0.0f);
	assert(airfoilFile.size());
	assert(rings.size() >= 2);

	for (U32 ringIdx = 1; ringIdx < rings.size(); ringIdx++)
		assert(rings[ringIdx].locationOnWing != rings[ringIdx - 1].locationOnWing);

	for (auto& ring : rings)
		assert(ring.chord > 0.0f);

	for (auto& section : sections)
		assert(section.dihedral <= 45.0f);
}

void Wing::GenerateAirfoils()
{
	Airfoil airfoil(airfoilFile + L".dat");
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoil.points)
			airfoils[ringIdx].push_back({ point.x, point.y, 0.0f });
	}
}

void Wing::ApplySweeps()
{
	for (U32 sectionIdx = 0; sectionIdx < sections.size(); ++sectionIdx)
	{
		auto sweep = XMConvertToRadians(sections[sectionIdx].sweep);
		for (auto& point : airfoils[sectionIdx + 1])
		{
			point.x = point.x * cos(sweep) + point.z * sin(sweep);
		}
	}
}

void Wing::ApplyDihedrals()
{
	for (U32 sectionIdx = 0; sectionIdx < sections.size(); ++sectionIdx)
	{
		auto dihedral = DegToRad(sections[sectionIdx].dihedral);
		for (auto& point : airfoils[sectionIdx + 1])
		{
			point.y = point.y * cos(dihedral) + point.z * sin(dihedral);
		}
	}
}

void Wing::ScaleSectionsByChord()
{
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		XMFLOAT3 scalingVec = { rings[ringIdx].chord, rings[ringIdx].chord, 1.0f };
		auto scalingMat = XMMatrixScalingFromVector(XMLoadFloat3(&scalingVec));

		auto rotMat = XMMatrixRotationZ(DegToRad(rings[ringIdx].incidenceAngle));

		for (auto& point : airfoils[ringIdx])
		{
			XMStoreFloat3(&point, XMVector3Transform(XMLoadFloat3(&point), scalingMat));
			XMStoreFloat3(&point, XMVector3Transform(XMLoadFloat3(&point), rotMat));
		}
	}
}

void Wing::PlaceRingsAlongWing()
{
	// Sort rings by location on wing 
	std::sort(rings.begin(), rings.end(), [](const WingRing& lhs, const WingRing& rhs) -> bool {
		return lhs.locationOnWing < rhs.locationOnWing;
	});

	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoils[ringIdx])
			point.z = length * rings[ringIdx].locationOnWing;
	}
}

void Wing::GenerateMeshVertsIndices(Mesh& mesh)
{
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoils[ringIdx])
		{
			mesh.verts.push_back(Vertex(point));
		}
	}

	auto ringSize = airfoils[0].size();
	for (U32 ringIdx = 0; ringIdx < rings.size() - 1; ringIdx++)
	{
		mesh.StitchRings(ringSize, ringSize * ringIdx,
								   ringSize * (ringIdx + 1));
	}
}

Mesh Wing::GenerateMesh()
{
	CheckConfigurationValidity();
	
	airfoils.resize(rings.size());

	GenerateAirfoils();
	ScaleSectionsByChord();
	PlaceRingsAlongWing();
	ApplySweeps();
	ApplyDihedrals();

	Mesh mesh;
	GenerateMeshVertsIndices(mesh);

	wingtip.Generate(mesh, airfoils[airfoils.size () - 1], (rings.size() - 1) * airfoils[0].size());

	mesh.GenerateNormals();

	return mesh;
}