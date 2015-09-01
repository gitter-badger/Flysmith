#pragma once
#include "PCH.h"
#include "Airfoil.h"
#include "StandardUnits.h"
using namespace DirectX;


class Wing
{
public:
	Wing(const std::wstring& airfoilFile)
		: m_airfoil(airfoilFile)
	{
	}

	// TODO: Scale taking into account the chord
	// TODO: Wing tip generation 
	// TODO: Control surfaces(separate?)
	// TODO: Sweep
	// TODO: Variable thickness 
	// TODO: Variable airfoil
	// TODO: Innards 
	Mesh GenerateMesh()
	{
		Mesh mesh;

		// Measured from one wing tip to the other(i.e. includes main body)
		F32 wingspan = MetersToD3DUnits(11.0f);
		F32 bodyDiameter = MetersToD3DUnits(2.0f);

		// 1. Generate ring configurations(for now all rings are the same).
		F32 wingWidth = (wingspan - bodyDiameter) / 2.0f;
		U32 numRings = 5;
		F32 ringOffsets[5] = { 0.0f, wingWidth / 4.0f, wingWidth / 2.0f, 3 * wingWidth / 4.0f, wingWidth };

		// 2. Generate vertices on rings 
		for (U32 ringIndex = 0; ringIndex < numRings; ringIndex++)
		{
			for (auto& point : m_airfoil.points)
			{
				XMFLOAT2 vert(point);
				mesh.verts.push_back({ { vert.x, vert.y, ringOffsets[ringIndex] }, { 0.0f, 0.0f, 0.0f, 1.0f } });
			}
		}

		// 3. Stitch
		for (U32 ringIndex = 0; ringIndex < numRings - 1; ringIndex++)
		{
			auto numPoints = m_airfoil.points.size();
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

		/*for (auto& foilPoint : points)
		{
			XMFLOAT2 vert(foilPoint);
			vert.x -= .5f;
			vert.y -= .5f;
			mesh.verts.push_back({ { vert.x, vert.y, 0.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } });
		}

		auto numPoints = points.size();
		for (U32 i = 1; i < numPoints; i++)
		{
			mesh.indices.push_back(i);
			mesh.indices.push_back(numPoints - i);
			mesh.indices.push_back(i - 1);
		}*/

		return mesh;
	}

private:
	Airfoil m_airfoil;
};