#include "PCH.h"
#include "Transform.h"


cuc::Transform::Transform()
	: m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position)
	: TransformNoScale(position)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation)
	: TransformNoScale(position, rotation)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, float pitch, float yaw, float roll)
	: TransformNoScale(position, pitch, yaw, roll)
	, m_scale(1.0f, 1.0f, 1.0f)
{
	CacheTransform();
}

cuc::Transform::Transform(const XMFLOAT3& position, const Quaternion& rotation, const XMFLOAT3& scale)
	: TransformNoScale(position, rotation)
	, m_scale(scale)
{
	CacheTransform();
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
									    m_rotation.GetMatrixFormXM() *
									    XMMatrixTranslationFromVector(XMLoadFloat3(&m_position)));
}
