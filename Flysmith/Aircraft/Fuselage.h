#pragma once
#include "Mesh.h"


struct FuselageRing
{
	float diameter;

	// Displacement from the aircraft's front
	float x;
	float y;
};

struct Fuselage
{
	void ReadFromFile(const std::wstring& filename);
	Mesh GenerateMesh();

	// TODO: Change to displacement from center
	std::vector<Vertex> GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront) const;

	std::vector<FuselageRing> rings;
};