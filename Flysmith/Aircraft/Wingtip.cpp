#include "PCH.h"
#include "Wingtip.h"
#include "Mesh.h"


Wingtip::Wingtip()
	: type(CUTOFF)
{
}

void Wingtip::Generate(Mesh& mesh, VertexArray& tipAirfoil, U32 tipVertexBegin)
{
	switch (type)
	{
	case CUTOFF:
		GenerateCutoff(mesh, tipAirfoil, tipVertexBegin);
		break;
	}
}

void Wingtip::GenerateCutoff(Mesh& mesh, VertexArray& tipAirfoil, U32 tipVertexBegin)
{
	// For whole wing
	auto numPoints = tipVertexBegin + tipAirfoil.size();

	for (U32 pointIdx = tipVertexBegin; pointIdx < numPoints - 1; ++pointIdx)
	{
		mesh.indices.push_back(pointIdx);
		mesh.indices.push_back(pointIdx + 1);
		mesh.indices.push_back(numPoints - 1 - (pointIdx - tipVertexBegin));
	}
}