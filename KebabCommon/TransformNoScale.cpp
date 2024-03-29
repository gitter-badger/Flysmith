#include "TransformNoScale.h"


XMFLOAT4X4 TransformNoScale::s_mirrorXMat = {
	1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, -1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

XMFLOAT4X4 TransformNoScale::s_mirrorYMat = {
	1.f, 0.f, 0.f, 0.f,
	0.f, -1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

XMFLOAT4X4 TransformNoScale::s_mirrorZMat = {
	-1.f, 0.f, 0.f, 0.f,
	0.f, 1.f, 0.f, 0.f,
	0.f, 0.f, 1.f, 0.f,
	0.f, 0.f, 0.f, 1.f
};

TransformNoScale::TransformNoScale()
	: m_position(0.0f, 0.0f, 0.0f)
	, m_rotation(0.0f, 0.0f, 0.0f)
	, m_bMirrorX(0.0f)
	, m_bMirrorY(0.0f)
	, m_bMirrorZ(0.0f)
{
	CacheTransform();
}

TransformNoScale::TransformNoScale(const XMFLOAT3& position)
	: m_position(position)
	, m_rotation(0.0f, 0.0f, 0.0f)
{
	CacheTransform();
}

TransformNoScale::TransformNoScale(const XMFLOAT3& position, const Quaternion& rotation)
	: m_position(position)
	, m_rotation(rotation)
{
	CacheTransform();
}

TransformNoScale::TransformNoScale(const XMFLOAT3& position, float pitch, float yaw, float roll)
	: m_position(position)
	, m_rotation(pitch, yaw, roll)
{
	CacheTransform();
}

const XMFLOAT4X4& TransformNoScale::GetMatrix() const
{
	return m_transformMatrix;
}

const XMMATRIX TransformNoScale::GetMatrixXM() const
{
	return XMLoadFloat4x4(&m_transformMatrix);
}

const Quaternion& TransformNoScale::GetRotationQuat() const
{
	return m_rotation;
}

const XMFLOAT3& TransformNoScale::GetPosition() const
{
	return m_position;
}

const XMVECTOR TransformNoScale::GetPositionXM() const
{
	return XMLoadFloat3(&m_position);
}

bool operator==(const TransformNoScale& lhs, const TransformNoScale& rhs)
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

bool operator!=(const TransformNoScale& lhs, const TransformNoScale& rhs)
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

void TransformNoScale::SetPosition(CXMVECTOR newPosition)
{
	XMStoreFloat3(&m_position, newPosition);
	CacheTransform();
}

void TransformNoScale::SetPosition(const DirectX::XMFLOAT3& newPosition)
{
	m_position = newPosition;
	CacheTransform();
}

void TransformNoScale::SetPosition(float x, float y, float z)
{
	m_position = { x, y, z };
	CacheTransform();
}

void TransformNoScale::SetRotation(CXMMATRIX rotMatrix)
{
	m_rotation.SetFromMatrix(rotMatrix);
	CacheTransform();
}

void TransformNoScale::SetRotation(const XMFLOAT4X4& rotMatrix)
{
	m_rotation.SetFromMatrix(rotMatrix);
	CacheTransform();
}

void TransformNoScale::SetRotation(const DirectX::XMFLOAT3& newRotation)
{
	m_rotation.SetFromEuler(newRotation);
	CacheTransform();
}

void TransformNoScale::SetRotation(float x, float y, float z)
{
	m_rotation.SetFromEuler(x, y, z);
	CacheTransform();
}

void TransformNoScale::SetRotation(const Quaternion& rotation)
{
	m_rotation = rotation;
	CacheTransform();
}

void TransformNoScale::Translate(CXMVECTOR other)
{
	XMStoreFloat3(&m_position, XMLoadFloat3(&m_position) + other);
	CacheTransform();
}

void TransformNoScale::Translate(const DirectX::XMFLOAT3& dPosition)
{
	m_position.x += dPosition.x;
	m_position.y += dPosition.y;
	m_position.z += dPosition.z;
	CacheTransform();
}

void TransformNoScale::Translate(float dX, float dY, float dZ)
{
	m_position.x += dX;
	m_position.y += dY;
	m_position.z += dZ;
	CacheTransform();
}

void TransformNoScale::TranslateX(float dX)
{
	m_position.x += dX;
	CacheTransform();
}

void TransformNoScale::TranslateY(float dY)
{
	m_position.y += dY;
	CacheTransform();
}

void TransformNoScale::TranslateZ(float dZ)
{
	m_position.z += dZ;
	CacheTransform();
}

void TransformNoScale::Rotate(const Quaternion& other)
{
	m_rotation.Concat(other);
	CacheTransform();
}

void TransformNoScale::RotateX(float dPitchAngle)
{
	m_rotation.Concat(xAxis, dPitchAngle);
	CacheTransform();
}

void TransformNoScale::RotateY(float dYawAngle)
{
	m_rotation.Concat(yAxis, dYawAngle);
	CacheTransform();
}

void TransformNoScale::RotateZ(float dRollAngle)
{
	m_rotation.Concat(zAxis, dRollAngle);
	CacheTransform();
}

void TransformNoScale::MirrorAlongX()
{
	m_bMirrorX = !m_bMirrorX;
	CacheTransform();
}

void TransformNoScale::MirrorAlongY()
{
	m_bMirrorY = !m_bMirrorY;
	CacheTransform();
}

void TransformNoScale::MirrorAlongZ()
{
	m_bMirrorZ = !m_bMirrorZ;
	CacheTransform();
}

void TransformNoScale::CacheTransform()
{
	XMMATRIX transform;

	// Rotate -> Translate
	transform = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position)) * m_rotation.GetMatrixFormXM();
	
	if (m_bMirrorX)
		transform *= XMLoadFloat4x4(&s_mirrorXMat);

	if (m_bMirrorY)
		transform *= XMLoadFloat4x4(&s_mirrorYMat);

	if (m_bMirrorZ)
		transform *= XMLoadFloat4x4(&s_mirrorZMat);

	XMStoreFloat4x4(&m_transformMatrix, transform);
}
