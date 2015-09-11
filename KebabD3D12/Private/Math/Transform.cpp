#include "PCH.h"
#include "Transform.h"


const Transform kDefaultTransform;

Transform::Transform()
	: m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

Transform::Transform(const XMFLOAT3& position)
	: TransformNoScale(position)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation)
	: TransformNoScale(position, rotation)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

Transform::Transform(const XMFLOAT3& position, float pitch, float yaw, float roll)
	: TransformNoScale(position, pitch, yaw, roll)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation, const XMFLOAT3& scale)
	: TransformNoScale(position, rotation)
	, m_scale(scale)
{
	CacheTransform();
}

const XMFLOAT3& Transform::GetScale() const
{
	return m_scale;
}

const XMVECTOR Transform::GetScaleXM() const
{
	return XMLoadFloat3(&m_scale);
}

bool operator==(const Transform& lhs, const Transform& rhs)
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

bool operator!=(const Transform& lhs, const Transform& rhs)
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

void Transform::SetScale(float byUniformScale)
{
	m_scale = { byUniformScale, byUniformScale, byUniformScale };
	CacheTransform();
}

void Transform::SetScale(const DirectX::XMFLOAT3& newScale)
{
	m_scale = newScale;
	CacheTransform();
}

void Transform::SetScale(float x, float y, float z)
{
	m_scale = { x, y, z };
	CacheTransform();
}

void Transform::Scale(float uniformScale)
{
	m_scale.x *= uniformScale;
	m_scale.y *= uniformScale;
	m_scale.z *= uniformScale;
	CacheTransform();
}

void Transform::Scale(const DirectX::XMFLOAT3& dScale)
{
	m_scale.x *= dScale.x;
	m_scale.y *= dScale.y;
	m_scale.z *= dScale.z;
	CacheTransform();
}

void Transform::Scale(float byX, float byY, float byZ)
{
	m_scale.x *= byX;
	m_scale.y *= byY;
	m_scale.z *= byZ;
	CacheTransform();
}

void Transform::ScaleX(float byX)
{
	m_scale.x *= byX;
	CacheTransform();
}

void Transform::ScaleY(float byY)
{
	m_scale.y *= byY;
	CacheTransform();
}

void Transform::ScaleZ(float byZ)
{
	m_scale.z *= byZ;
	CacheTransform();
}

void Transform::CacheTransform()
{
	XMMATRIX transform;

	// Scale -> Rotate -> Translate
	transform = XMMatrixTranslationFromVector(XMLoadFloat3(&m_position))*
				m_rotation.GetMatrixFormXM() *
				XMMatrixScalingFromVector(XMLoadFloat3(&m_scale));

	if (m_bMirrorX)
		transform *= XMLoadFloat4x4(&s_mirrorXMat);

	if (m_bMirrorY)
		transform *= XMLoadFloat4x4(&s_mirrorYMat);

	if (m_bMirrorZ)
		transform *= XMLoadFloat4x4(&s_mirrorZMat);

	XMStoreFloat4x4(&m_transformMatrix, transform);
}
