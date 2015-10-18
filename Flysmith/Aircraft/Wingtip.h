#pragma once
#include "Vertex.h"


struct Mesh;

class Wingtip
{
public:
	enum Type
	{
		CUTOFF,
		ROUNDED,
		SHARP,
		HOERNER,
		DROOPED,
		ENDPLATE,
		WINGLET
	} type;

	// For now, it has to match the wing's airfoil at the tip.
	VertexArray tipAirfoil;
	
public:
	Wingtip();
	Mesh Generate();

private:
	void GenerateCutoff(Mesh& meshOut);
};