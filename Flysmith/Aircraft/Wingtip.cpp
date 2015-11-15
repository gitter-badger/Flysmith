#include "PCH.h"
#include "Wingtip.h"
#include "Mesh.h"


const std::map<std::string, Wingtip::Type> Wingtip::s_typeNames = {
	{ "Cutoff", CUTOFF },
	{ "Rounded", ROUNDED },
	{ "Sharp", SHARP },
	{ "Hoerner", HOERNER },
	{ "Drooped", DROOPED },
	{ "Endplate", ENDPLATE },
	{ "Winglet", WINGLET }
};

Wingtip::Wingtip() : type(CUTOFF)
{
}

Mesh Wingtip::Generate()
{
	Mesh mesh;

	// Any wingtip device will at least contain the wing...tip's airfoil's verts.
	for (auto& vertPos : tipAirfoil)
	{
		mesh.verts.push_back(Vertex(vertPos));
	}

	// Generate the rest of the geometry(if needed) and set connectivity, depending on the set type.
	switch (type)
	{
	case CUTOFF:
		GenerateCutoff(mesh);
		break;
	case ROUNDED:
	case SHARP:
	case HOERNER:
	case DROOPED:
	case ENDPLATE:
	case WINGLET:
		break;	
	}

	return mesh;
}

void Wingtip::GenerateCutoff(Mesh& meshOut) const
{
	auto numPoints = tipAirfoil.size();

	// There's no geometry generation needed for the cutoff wingtip, 
	// therefore only connectivity info is handled.
	for (U32 pointIdx = 0; pointIdx < numPoints - 1; ++pointIdx)
	{
		meshOut.indices.push_back(pointIdx);
		meshOut.indices.push_back(pointIdx + 1);
		meshOut.indices.push_back(numPoints - 1 - pointIdx);
	}
}