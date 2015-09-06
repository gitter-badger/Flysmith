#pragma once
#include "Mesh.h"


struct Fuselage
{
	Mesh GenerateMesh();

	// TODO: Change to displacement from center
	std::vector<Vertex> GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront);
};