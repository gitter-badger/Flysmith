#include "PCH.h"
#include "Transform.h"


cuc::Transform::Transform()
	: position(0.0f, 0.0f, 0.0f)
	, rotation(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

DirectX::XMFLOAT4X4 cuc::Transform::GetMatrix() const
{
	return transformMatrix;
}

void cuc::Transform::SetPosition(const DirectX::XMFLOAT3& newPosition)
{
	position = newPosition;
	CacheTransform();
}

void cuc::Transform::SetPosition(float x, float y, float z)
{
	position = { x, y, z };
	CacheTransform();
}

void cuc::Transform::SetRotation(const DirectX::XMFLOAT3& newRotation)
{
	rotation = newRotation;
	CacheTransform();
}

void cuc::Transform::SetRotation(float x, float y, float z)
{
	rotation = { x, y, z };
	CacheTransform();
}

void cuc::Transform::SetScale(const DirectX::XMFLOAT3& newScale)
{
	scale = newScale;
	CacheTransform();
}

void cuc::Transform::SetScale(float x, float y, float z)
{
	scale = { x, y, z };
	CacheTransform();
}

void cuc::Transform::Translate(const DirectX::XMFLOAT3& dPosition)
{
	position.x += dPosition.x;
	position.y += dPosition.y;
	position.z += dPosition.z;
	CacheTransform();
}

void cuc::Transform::Translate(float dX, float dY, float dZ)
{
	position.x += dX;
	position.y += dY;
	position.z += dZ;
	CacheTransform();
}

void cuc::Transform::TranslateX(float dX)
{
	position.x += dX;
	CacheTransform();
}

void cuc::Transform::TranslateY(float dY)
{
	position.y += dY;
	CacheTransform();
}

void cuc::Transform::TranslateZ(float dZ)
{
	position.z += dZ;
	CacheTransform();
}

void cuc::Transform::Rotate(const DirectX::XMFLOAT3& dRotation)
{
	rotation.x += dRotation.x;
	rotation.y += dRotation.y;
	rotation.z += dRotation.z;
	CacheTransform();
}

void cuc::Transform::Rotate(float dPitchAngle, float dYawAngle, float dRollAngle)
{
	rotation.x += dPitchAngle;
	rotation.y += dYawAngle;
	rotation.z += dRollAngle;
	CacheTransform();
}

void cuc::Transform::RotateX(float dPitchAngle)
{
	rotation.x += dPitchAngle;
	CacheTransform();
}

void cuc::Transform::RotateY(float dYawAngle)
{
	rotation.y += dYawAngle;
	CacheTransform();
}

void cuc::Transform::RotateZ(float dRollAngle)
{
	rotation.z += dRollAngle;
	CacheTransform();
}

void cuc::Transform::Scale(const DirectX::XMFLOAT3& dScale)
{
	scale.x *= dScale.x;
	scale.y *= dScale.y;
	scale.z *= dScale.z;
	CacheTransform();
}

void cuc::Transform::Scale(float byX, float byY, float byZ)
{
	scale.x *= byX;
	scale.y *= byY;
	scale.z *= byZ;
	CacheTransform();
}

void cuc::Transform::ScaleX(float byX)
{
	scale.x *= byX;
	CacheTransform();
}

void cuc::Transform::ScaleY(float byY)
{
	scale.y *= byY;
	CacheTransform();
}

void cuc::Transform::ScaleZ(float byZ)
{
	scale.z *= byZ;
	CacheTransform();
}

void cuc::Transform::CacheTransform()
{
	// Scale -> Rotate -> Translate
	XMStoreFloat4x4(&transformMatrix, XMMatrixScalingFromVector(XMLoadFloat3(&scale)) *
									  XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rotation)) *
									  XMMatrixTranslationFromVector(XMLoadFloat3(&position)));
}
