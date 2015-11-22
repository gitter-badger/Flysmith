#include "Vertex.h"
using namespace DirectX;


Vertex::Vertex(const Vector2& position_)
	: position(position_)
{
}

Vertex::Vertex(const Vector3& position_)
	: position(position_)
{
}

Vertex::Vertex(const Vector3& position_, const Vector3& normal_)
	: position(position_)
	, normal(normal_)
{
}
