#pragma once
#include "PCH.h"
#include "Types.h"
#include "../KebabD3D12/Public/Vertex.h"


struct Mesh
{
	std::vector<U32> indices;
	std::vector<Vertex> verts;
};