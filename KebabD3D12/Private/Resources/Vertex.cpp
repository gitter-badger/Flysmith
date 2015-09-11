#include "PCH.h"
#include "Vertex.h"
using namespace DirectX;


Vertex::Vertex()
	: position(0.0f, 0.0f, 0.0f)
	, normal(0.0f, 0.0f, 0.0f)
{
}

Vertex::Vertex(const XMFLOAT3& position_)
	: position(position_)
	, normal(0.0f, 0.0f, 0.0f)
{
}

Vertex::Vertex(const XMFLOAT3& position_, const XMFLOAT3& normal_)
	: position(position_)
	, normal(normal_)
{
}

Vertex::Vertex(const XMFLOAT2& position_)
	: position(position_.x, position_.y, 0.0f)
	, normal(0.0f, 0.0f, 0.0f)
{
}