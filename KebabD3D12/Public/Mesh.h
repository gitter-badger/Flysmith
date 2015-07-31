#pragma once
#include "PublicDef.h"
#include <vector>
#include <DirectXMath.h>


namespace cuc
{
	class KEBAB12_API Vertex
	{
	public:
		float position[3];
		float color[4];
	};

	class KEBAB12_API Mesh
	{
	public:
		std::vector<Vertex> verts;
		std::vector<U32> indices;

		void AddVertex(float x, float y);
		void AddVertex(DirectX::XMFLOAT2 pos);
		// CCW order
		void SetTriangle(U32 v0, U32 v1, U32 v2);
	};
}