#include "PCH.h"
#include "Transform.h"


cuc::Transform::Transform()
	: m_position(0.0f, 0.0f, 0.0f)
	, m_rotation(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position)
	: m_position(position)
	, m_rotation(0.0f, 0.0f, 0.0f)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, const XMFLOAT3& rotation)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, float pitch, float yaw, float roll)
	: m_position(position)
	, m_rotation(pitch, yaw, roll)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale)
	: m_position(position)
	, m_rotation(rotation)
	, m_scale(scale)
{
	CacheTransform();
}

const XMFLOAT4X4& cuc::Transform::GetMatrix() const
{
	return m_transformMatrix;
}

const XMMATRIX cuc::Transform::GetMatrixXM() const
{
	return XMLoadFloat4x4(&m_transformMatrix);
}

const XMFLOAT3& cuc::Transform::GetPosition() const
{
	return m_position;
}

const XMVECTOR cuc::Transform::GetPositionXM() const
{
	return XMLoadFloat3(&m_position);
}

const XMFLOAT3& cuc::Transform::GetRotation() const
{
	return m_rotation;
}

const XMVECTOR cuc::Transform::GetRotationXM() const
{
	return XMLoadFloat3(&m_rotation);
}

const XMFLOAT3& cuc::Transform::GetScale() const
{
	return m_scale;
}

const XMVECTOR cuc::Transform::GetScaleXM() const
{
	return XMLoadFloat3(&m_scale);
}

bool cuc::operator==(const cuc::Transform& lhs, const cuc::Transform& rhs)
{
	for (U8 line = 0; line < 4; line++)
	{
		for (U8 col = 0; col < 4; col++)
		{
			if (lhs.m_transformMatrix(line, col) != rhs.m_transformMatrix(line, col))
				return false;
		}
	}

	return true;
}

bool cuc::operator!=(const cuc::Transform& lhs, const cuc::Transform& rhs)
{
	for (U8 line = 0; line < 4; line++)
	{
		for (U8 col = 0; col < 4; col++)
		{
			if (lhs.m_transformMatrix(line, col) != rhs.m_transformMatrix(line, col))
				return true;
		}
	}

	return false;
}

void cuc::Transform::SetPosition(const DirectX::XMFLOAT3& newPosition)
{
	m_position = newPosition;
	CacheTransform();
}

void cuc::Transform::SetPosition(float x, float y, float z)
{
	m_position = { x, y, z };
	CacheTransform();
}

void cuc::Transform::SetRotation(const DirectX::XMFLOAT3& newRotation)
{
	m_rotation = newRotation;
	CacheTransform();
}

void cuc::Transform::SetRotation(float x, float y, float z)
{
	m_rotation = { x, y, z };
	CacheTransform();
}

void cuc::Transform::SetScale(float byUniformScale)
{
	m_scale = { byUniformScale, byUniformScale, byUniformScale };
	CacheTransform();
}

void cuc::Transform::SetScale(const DirectX::XMFLOAT3& newScale)
{
	m_scale = newScale;
	CacheTransform();
}

void cuc::Transform::SetScale(float x, float y, float z)
{
	m_scale = { x, y, z };
	CacheTransform();
}

void cuc::Transform::Translate(const DirectX::XMFLOAT3& dPosition)
{
	m_position.x += dPosition.x;
	m_position.y += dPosition.y;
	m_position.z += dPosition.z;
	CacheTransform();
}

void cuc::Transform::Translate(float dX, float dY, float dZ)
{
	m_position.x += dX;
	m_position.y += dY;
	m_position.z += dZ;
	CacheTransform();
}

void cuc::Transform::TranslateX(float dX)
{
	m_position.x += dX;
	CacheTransform();
}

void cuc::Transform::TranslateY(float dY)
{
	m_position.y += dY;
	CacheTransform();
}

void cuc::Transform::TranslateZ(float dZ)
{
	m_position.z += dZ;
	CacheTransform();
}

void cuc::Transform::Rotate(const DirectX::XMFLOAT3& dRotation)
{
	m_rotation.x += dRotation.x;
	m_rotation.y += dRotation.y;
	m_rotation.z += dRotation.z;
	CacheTransform();
}

void cuc::Transform::Rotate(float dPitchAngle, float dYawAngle, float dRollAngle)
{
	m_rotation.x += dPitchAngle;
	m_rotation.y += dYawAngle;
	m_rotation.z += dRollAngle;
	CacheTransform();
}

void cuc::Transform::RotateX(float dPitchAngle)
{
	m_rotation.x += dPitchAngle;
	CacheTransform();
}

void cuc::Transform::RotateY(float dYawAngle)
{
	m_rotation.y += dYawAngle;
	CacheTransform();
}

void cuc::Transform::RotateZ(float dRollAngle)
{
	m_rotation.z += dRollAngle;
	CacheTransform();
}

void cuc::Transform::Scale(float uniformScale)
{
	m_scale.x *= uniformScale;
	m_scale.y *= uniformScale;
	m_scale.z *= uniformScale;
	CacheTransform();
}

void cuc::Transform::Scale(const DirectX::XMFLOAT3& dScale)
{
	m_scale.x *= dScale.x;
	m_scale.y *= dScale.y;
	m_scale.z *= dScale.z;
	CacheTransform();
}

void cuc::Transform::Scale(float byX, float byY, float byZ)
{
	m_scale.x *= byX;
	m_scale.y *= byY;
	m_scale.z *= byZ;
	CacheTransform();
}

void cuc::Transform::ScaleX(float byX)
{
	m_scale.x *= byX;
	CacheTransform();
}

void cuc::Transform::ScaleY(float byY)
{
	m_scale.y *= byY;
	CacheTransform();
}

void cuc::Transform::ScaleZ(float byZ)
{
	m_scale.z *= byZ;
	CacheTransform();
}

void cuc::Transform::CacheTransform()
{
	// Scale -> Rotate -> Translate
	XMStoreFloat4x4(&m_transformMatrix, XMMatrixScalingFromVector(XMLoadFloat3(&m_scale)) *
									    XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_rotation)) *
									    XMMatrixTranslationFromVector(XMLoadFloat3(&m_position)));
}
