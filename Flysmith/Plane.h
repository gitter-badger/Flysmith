#pragma once
#include "Wing.h"


class Plane
{
public:
	Plane();

	// In local space, the plane is looking down the positive z-axis
	// @lengthOffset - distance between wing pair midpoint and center of aircraft, on the z-axis
	// @heightOffset - distance between wing pair midpoint and center of aircraft, on the y-axis
	void AddWingPair(Wing wing, F32 lengthOffset, F32 heightOffset, F32 separationWidth);
	Mesh& GetMesh();

private:
	Mesh m_mesh;
	U32 m_numPairs;
};