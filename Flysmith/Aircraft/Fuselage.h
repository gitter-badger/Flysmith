#pragma once
#include "Mesh.h"


struct Fuselage
{
	Mesh GenerateMesh(const std::wstring& path);

	// TODO: Change to displacement from center
	std::vector<Vertex> GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront);
};