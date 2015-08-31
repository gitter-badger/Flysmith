#pragma once
#include "PCH.h"
#include "Airfoil.h"
using namespace DirectX;


class Wing
{
public:
	Wing(const std::wstring& airfoilFile)
		: m_airfoil(airfoilFile)
	{
	}

	Mesh GenerateMesh()
	{
		Mesh mesh;

		// 1. Generate ring configurations(for now all rings are the same).
		U32 numRings = 2;
		F32 ringOffsets[2] = { 0.0f, 0.5f };

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