#include "PCH.h"
#include "Vector.h"


Vector2::Vector2() : x(0.0f), y(0.0f) {}

Vector2::Vector2(float x_, float y_) : x(x_), y(y_) {}

Vector2::Vector2(float uniformValue) : x(uniformValue), y(uniformValue) {}

Vector3::Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

Vector3::Vector3(float uniformValue) : x(uniformValue), y(uniformValue), z(uniformValue) {}

Vector3::Vector3(Vector2 vec, float z_) : x(vec.x), y(vec.y), z(z_) {}

DirectX::XMVECTOR Vector2::GetXMVec()
{
	return DirectX::XMVectorSet(x, y, 0.0f, 0.0f);
}

void Vector2::Set(DirectX::CXMVECTOR vec)
{
	x = DirectX::XMVectorGetX(vec);
	y = DirectX::XMVectorGetY(vec);
}

DirectX::XMVECTOR Vector3::GetXMVec()
{
	return DirectX::XMVectorSet(x, y, z, 0.0f);
}

void Vector3::Set(DirectX::CXMVECTOR vec)
{
	x = DirectX::XMVectorGetX(vec);
	y = DirectX::XMVectorGetY(vec);
	z = DirectX::XMVectorGetZ(vec);
}
