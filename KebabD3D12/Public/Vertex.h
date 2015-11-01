#pragma once
#include "PublicDef.h"
#include "Vector.h"


struct KEBAB12_API Vertex
{
	Vector3 position;
	Vector3 normal;

	Vertex(const Vector2& position);
	Vertex(const Vector3& position);
	Vertex(const Vector3& position, const Vector3& normal);
};


using VertexArray = std::vector<Vertex>; 
using IndexArray  = std::vector<U32>;