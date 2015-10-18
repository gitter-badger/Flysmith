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

	static const std::map<std::string, Type> s_typeNames;

	// For now, it has to match the wing's airfoil at the tip.
	VertexArray tipAirfoil;
	
public:
	Wingtip();
	Mesh Generate();

private:
	void GenerateCutoff(Mesh& meshOut);
};