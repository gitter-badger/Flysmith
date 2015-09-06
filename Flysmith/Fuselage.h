#pragma once
#include "Mesh.h"


struct FuselageRing
{
	enum Shape
	{
		CIRCULAR
	};
};

struct Fuselage
{
	Mesh GenerateMesh()
	{
		U32 numRings = 5;

		// Generate vertex data
		Mesh mesh;
		auto ring3 = GenerateCircularRing(0.1f, { 0.0f, 0.0f, -1.2f });
		auto ring0 = GenerateCircularRing(0.5f, { 0.0f, 0.0f, -1.0f });
		auto ring1 = GenerateCircularRing(1.0f, { 0.0f, 0.0f, 0.0f });
		auto ring2 = GenerateCircularRing(1.0f, { 0.0f, 0.0f, 2.0f });
		auto ring4 = GenerateCircularRing(0.1f, { 0.0f, 0.0f, 5.0f });
		
		mesh.verts.insert(mesh.verts.end(), ring3.begin(), ring3.end());
		mesh.verts.insert(mesh.verts.end(), ring0.begin(), ring0.end());
		mesh.verts.insert(mesh.verts.end(), ring1.begin(), ring1.end());
		mesh.verts.insert(mesh.verts.end(), ring2.begin(), ring2.end());
		mesh.verts.insert(mesh.verts.end(), ring4.begin(), ring4.end());

		// *Stable* sort by z coord

		// Stitch
		for (U32 ringIndex = 0; ringIndex < numRings - 1; ringIndex++)
		{
			auto numPoints = mesh.verts.size() / numRings;
			auto ringOffset1 = numPoints * ringIndex;
			auto ringOffset2 = numPoints * (ringIndex + 1);

			for (U32 i = 0; i < numPoints - 1; i++)
			{
				mesh.indices.push_back(ringOffset1 + i);
				mesh.indices.push_back(ringOffset2 + i + 1);
				mesh.indices.push_back(ringOffset2 + i);
			}

			for (U32 i = 0; i < numPoints - 1; i++)
			{
				mesh.indices.push_back(ringOffset1 + i);
				mesh.indices.push_back(ringOffset1 + i + 1);
				mesh.indices.push_back(ringOffset2 + i + 1);
			}
		}

		mesh.GenerateNormals();

		return mesh;
	}

	// TODO: Change to displacement from center
	std::vector<Vertex> GenerateCircularRing(F32 diameter, XMFLOAT3 dispFromFront)
	{
		F32 radius = diameter / 2.0f;

		std::vector<Vertex> verts;

		for (F32 angle = 0; angle <= XM_2PI; angle += 0.1f)
		{
			XMFLOAT3 pos(radius * cos(angle), 
						 radius * sin(angle) + dispFromFront.y,
						 dispFromFront.z);
			XMFLOAT3 normal(0.0f, 0.0f, 0.0f);
			verts.push_back({ pos, normal });
		}

		return verts;
	}
};