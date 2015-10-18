#pragma once
#include "PublicDef.h"


struct KEBAB12_API DirectX::XMFLOAT3;


struct KEBAB12_API Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;

	Vertex();
	Vertex(const DirectX::XMFLOAT2& position);
	Vertex(const DirectX::XMFLOAT3& position);
	Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal);
};


using VertexArray = std::vector<Vertex>; 
using IndexArray  = std::vector<U32>;