#pragma once
#include "PCH.h"
#include "../KebabCommon/Vertex.h"
using namespace DirectX;


struct Mesh
{
	std::vector<U32> indices;
	std::vector<Vertex> verts;

	// Specify the verts in counter-clockwise order:
	// v2 --- v1
	// |    /  |
	// | I /   |
	// |  / II |
	// | /     |
	// v3 --- v0
	void SetQuadFace(U32 v0, U32 v1, U32 v2, U32 v3);
	void GenerateNormals();

	// TODO: Support ring pairs with different edge counts 
	void StitchRings(U32 numVertsInRing, U32 firstRingStart, U32 secondRingStart);
};