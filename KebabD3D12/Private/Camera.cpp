#include "PCH.h"
#include "Camera.h"
using namespace cuc;


Camera::Camera(float aspectRatio, float fov, float nearPlane, float farPlane)
	: m_aspectRatio(aspectRatio)
	, m_fieldOfView(fov)
	, m_nearPlane(nearPlane)
	, m_farPlane(farPlane)
	, m_lookDirection(0.0f, 0.0f, 1.0f)
	, m_upDirection(0.0f, 1.0f, 0.0f)
{
	CacheViewProjMatrices();
}

Camera::~Camera()
{
}

const XMFLOAT4X4& Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const XMMATRIX Camera::GetViewMatrixXM() const
{
	return XMLoadFloat4x4(&m_viewMatrix);
}

const XMFLOAT4X4& Camera::GetProjMatrix() const
{
	return m_projMatrix;
}

const XMMATRIX Camera::GetProjMatrixXM() const
{
	return XMLoadFloat4x4(&m_projMatrix);
}

const XMFLOAT4X4& Camera::GetViewProjMatrix() const
{
	return m_viewProjMatrix;
}

const XMMATRIX Camera::GetViewProjMatrixXM() const
{
	return XMLoadFloat4x4(&m_viewProjMatrix);
}

void cuc::Camera::SetTransform(const Transform& transform)
{
	if (transform != m_transform)
	{
		m_transform = transform;
		CacheViewProjMatrices();
	}
}

void cuc::Camera::SetPosition(const XMFLOAT3& newPosition)
{
	m_transform.SetPosition(newPosition);
	CacheViewProjMatrices();
}

void cuc::Camera::CacheViewProjMatrices()
{
	auto viewMat = XMMatrixLookToLH(XMLoadFloat3(&m_transform.GetPosition()),
									XMLoadFloat3(&m_lookDirection),
									XMLoadFloat3(&m_upDirection));

	// Rarely updated. To split.
	auto projMat = XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
	
	XMStoreFloat4x4(&m_viewMatrix, viewMat);
	XMStoreFloat4x4(&m_projMatrix, projMat);
	XMStoreFloat4x4(&m_viewProjMatrix, viewMat * projMat);
}