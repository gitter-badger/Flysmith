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

void Wing::AttachWingtip(Mesh& meshOut)
{
	// Set the wingtip device's base airfoil to the wing's last ring(tip)'s airfoil
	wingtip.tipAirfoil = airfoils[rings.size() - 1];
	
	// Generate wingtip device 
	auto wingtipMesh = wingtip.Generate();

	// For now, assume the wingtip device and the wing's tip have matching airfoil data
	// Stitching is done by adding any additional vertices besides the last ring's vertices already in place
	// and merging(and offsetting) the connectivity data.
	
	// Merge and offset connectivity info
	auto offset = meshOut.verts.size() - airfoils[rings.size() - 1].size();

	for (U32 vertIdx = 0; vertIdx < wingtipMesh.indices.size(); vertIdx++)
	{
		meshOut.indices.push_back(wingtipMesh.indices[vertIdx] + offset);
	}

	// Add additional verts
	auto numAdditionalVerts = wingtipMesh.verts.size() - airfoils[rings.size() - 1].size();
	meshOut.verts.reserve(meshOut.verts.size() + numAdditionalVerts);
	
	for (U32 vertIdx = airfoils[rings.size() - 1].size(); vertIdx < wingtipMesh.verts.size(); vertIdx++)
	{
		meshOut.verts.push_back(wingtipMesh.verts[vertIdx]);
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

	AttachWingtip(mesh);

	mesh.GenerateNormals();

	return mesh;
}