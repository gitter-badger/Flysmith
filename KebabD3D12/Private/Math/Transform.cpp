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

cuc::Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation)
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

cuc::Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation, const XMFLOAT3& scale)
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

const XMMATRIX cuc::Transform::GetRotationMatrixXM() const
{
	return m_rotation.GetMatrixXM();
}

const XMFLOAT3& cuc::Transform::GetPosition() const
{
	return m_position;
}

const XMVECTOR cuc::Transform::GetPositionXM() const
{
	return XMLoadFloat3(&m_position);
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

void cuc::Transform::SetPosition(CXMVECTOR newPosition)
{
	XMStoreFloat3(&m_position, newPosition);
	CacheTransform();
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

void cuc::Transform::SetRotation(CXMMATRIX rotMatrix)
{
	m_rotation.SetFromMatrix(rotMatrix);
	CacheTransform();
}

void cuc::Transform::SetRotation(const XMFLOAT4X4& rotMatrix)
{
	m_rotation.SetFromMatrix(rotMatrix);
	CacheTransform();
}

void cuc::Transform::SetRotation(const DirectX::XMFLOAT3& newRotation)
{
	m_rotation.SetFromEuler(newRotation);
	CacheTransform();
}

void cuc::Transform::SetRotation(float x, float y, float z)
{
	m_rotation.SetFromEuler(x, y, z);
	CacheTransform();
}

void cuc::Transform::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
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

void cuc::Transform::Translate(CXMVECTOR other)
{
	XMStoreFloat3(&m_position, XMLoadFloat3(&m_position) + other);
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

void cuc::Transform::Rotate(const Quaternion& other)
{
	m_rotation.Concat(other);
	CacheTransform();
}

void cuc::Transform::RotateX(float dPitchAngle)
{
	m_rotation.Concat(xAxis, dPitchAngle);
	CacheTransform();
}

void cuc::Transform::RotateY(float dYawAngle)
{
	m_rotation.Concat(yAxis, dYawAngle);
	CacheTransform();
}

void cuc::Transform::RotateZ(float dRollAngle)
{
	m_rotation.Concat(zAxis, dRollAngle);
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
									    m_rotation.GetMatrixXM() *
									    XMMatrixTranslationFromVector(XMLoadFloat3(&m_position)));
}
