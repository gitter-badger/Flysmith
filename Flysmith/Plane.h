#pragma once
#include "Wing.h"


class Plane
{
public:
	// In local space, the plane is looking down the positive z-axis
	// @lengthOffset - distance between wing pair midpoint and center of aircraft, on the z-axis
	// @heightOffset - distance between wing pair midpoint and center of aircraft, on the y-axis
	void AddWingPair(Wing wing, F32 lengthOffset, F32 heightOffset, F32 separationWidth)
	{
		auto rightWing = wing.GetMesh();
		for (auto& vert : rightWing.verts)
		{
			vert.position.z += separationWidth / 2.0f;
			vert.position.y += heightOffset;
		}

		for (auto& index : rightWing.indices)
			index += m_numPairs * rightWing.verts.size() * 2;

		auto leftWing = rightWing;

		// Mirror on the x-axis(currently the chord lines are parallel to the x-axis)
		for (auto& vert : leftWing.verts)
			vert.position.z = -vert.position.z;

		for (auto& index : leftWing.indices)
			index += leftWing.verts.size();

		m_mesh.verts.insert(m_mesh.verts.end(), rightWing.verts.begin(), rightWing.verts.end());
		m_mesh.verts.insert(m_mesh.verts.end(), leftWing.verts.begin(), leftWing.verts.end());
		m_mesh.indices.insert(m_mesh.indices.end(), rightWing.indices.begin(), rightWing.indices.end());
		m_mesh.indices.insert(m_mesh.indices.end(), leftWing.indices.begin(), leftWing.indices.end());

		m_mesh.GenerateNormals();

		m_numPairs++;
	}

	Mesh& GetMesh()
	{
		return m_mesh;
	}

private:
	Mesh m_mesh;
	U32 m_numPairs = 0;
	//std::vector<Wing> m_wingPairs;
};