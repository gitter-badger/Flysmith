#include "PCH.h"
#include "Wing.h"
#include "Mesh.h"
#include "json.hpp"
#include "Airfoil.h"
#include "Variant.h"
#include "Math\AngleMath.h"
#include "Resources\AssetLocator.h"


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

bool Wing::IsConfigValid()
{
	if (length < 0.0f)
	{
		g_logger.Write("Attempting to create wing of length " + std::to_string(length) + "m. It must be larger than 0m.");
		return false;
	}

	if (!airfoilFile.size())
	{
		g_logger.Write("Attempting to create wing with no airfoil file provided.");
		return false;
	}

	if (rings.size() < 2)
	{
		g_logger.Write("Attempting to create wing with " + std::to_string(rings.size()) + " rings. There must be at least 2 rings.");
		return false;
	}

	for (size_t ringIdx = 1; ringIdx < rings.size(); ++ringIdx)
	{
		if (rings[ringIdx].locationOnWing == rings[ringIdx - 1].locationOnWing)
		{
			g_logger.Write("Attempting to create wing with rings " + std::to_string(ringIdx) + " and " + std::to_string(ringIdx - 1) + " overlapping.");
			return false;
		}
	}

	for (size_t ringIdx = 1; ringIdx < rings.size(); ++ringIdx)
	{
		if (rings[ringIdx].chord < 0.0f)
		{
			g_logger.Write("Attempting to create wing with ring " + std::to_string(ringIdx) + " of chord "
						   + std::to_string(rings[ringIdx].chord) + "m while it must be larger than 0m.");
			return false;
		}
	}

	for (size_t sectionIdx = 0; sectionIdx < sections.size(); ++sectionIdx)
	{
		if (sections[sectionIdx].dihedral > 45.0f)
		{
			g_logger.Write("Attempting to create wing with section " + std::to_string(sectionIdx) + " having a dihedral of "
						   + std::to_string(sections[sectionIdx].dihedral) + " degrees. It must be less or equal than 45 degrees.");
			return false;
		}
	}

	return true;
}

void Wing::GenerateAirfoils()
{
	Airfoil airfoil(airfoilFile + L".dat");
	for (U32 ringIdx = 0; ringIdx < rings.size(); ringIdx++)
	{
		for (auto& point : airfoil.points)
			airfoils[ringIdx].push_back(Vertex({ point.x, point.y, 0.0f }));
	}
}

void Wing::ApplySweeps()
{
	for (U32 sectionIdx = 0; sectionIdx < sections.size(); ++sectionIdx)
	{
		auto sweep = XMConvertToRadians(sections[sectionIdx].sweep);
		for (auto& point : airfoils[sectionIdx + 1])
		{
			auto& pointPos = point.position;
			pointPos.x = pointPos.x * cos(sweep) + pointPos.z * sin(sweep);
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
			auto& pointPos = point.position;
			pointPos.y = pointPos.y * cos(dihedral) + pointPos.z * sin(dihedral);
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
			auto& pointPos = point.position;
			XMStoreFloat3(&pointPos, XMVector3Transform(XMLoadFloat3(&pointPos), scalingMat));
			XMStoreFloat3(&pointPos, XMVector3Transform(XMLoadFloat3(&pointPos), rotMat));
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
		{
			auto& pointPos = point.position;
			pointPos.z = length * rings[ringIdx].locationOnWing;
		}
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
	if (!IsConfigValid())
	{
		MessageBoxA(NULL, "Wing creation failed. Check log file for more details.", NULL, MB_OK);
	}
	
	airfoils.resize(rings.size());

	GenerateAirfoils();
	ScaleSectionsByChord();
	PlaceRingsAlongWing();
	ApplySweeps();
	ApplyDihedrals();

	Mesh mesh;
	OutputDebugStringA(std::to_string(mesh.verts.size()).c_str());
	OutputDebugStringA("\n");
	GenerateMeshVertsIndices(mesh);
	OutputDebugStringA(std::to_string(mesh.verts.size()).c_str());
	OutputDebugStringA("\n");

	AttachWingtip(mesh);

	mesh.GenerateNormals();

	return mesh;
}

void Wing::ReadFromFile(const std::wstring& filename)
{
	// TODO: Validity checking 

	AssetLocator al;

	std::ifstream wingfile(al.GetAssetDirectory(AssetType::WINGS) + filename + L".json");
	std::string wingdata;
	std::string token;
	while (wingfile >> token)
		wingdata += token;

	auto config = nlohmann::json::parse(wingdata);

	std::string wingFileTemp = config["airfoil"];
	airfoilFile = std::wstring(wingFileTemp.begin(), wingFileTemp.end());
	length = config["length"];
	std::string wingtipTypeName = config["wingtip"];
	wingtip.type = Wingtip::s_typeNames.at(wingtipTypeName);

	auto ringArray = config["rings"];
	for (U32 i = 0; i < ringArray.size(); i++)
	{
		auto ringConfig = ringArray[i];

		WingRing ring;
		ring.chord = ringConfig["chord"];
		ring.locationOnWing = ringConfig["location_on_wing"];
		ring.incidenceAngle = DegMinSecToDecimal(ringConfig["incidence_angle_deg"], ringConfig["incidence_angle_min"]);
		rings.push_back(ring);
	}

	auto sectionArray = config["sections"];
	for (U32 i = 0; i < sectionArray.size(); i++)
	{
		auto section = sectionArray[i];

		sections.push_back(WingSection());
		sections[sections.size() - 1].sweep = section["sweep"];
		sections[sections.size() - 1].dihedral = DegMinSecToDecimal(section["dihedral_deg"], section["dihedral_min"]);
	}
}
