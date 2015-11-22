#include "PCH.h"
#include "Camera.h"
#include <DirectXMath.h>


Camera::Camera(float viewWidth, float viewHeight, float fovY, float nearPlane, float farPlane)
	: m_projMatrix(viewWidth, viewHeight, fovY, nearPlane, farPlane)
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

const XMFLOAT4X4& Camera::GetViewProjMatrix() const
{
	return m_viewProjMatrix;
}

const XMMATRIX Camera::GetViewProjMatrixXM() const
{
	return XMLoadFloat4x4(&m_viewProjMatrix);
}

void Camera::Update(const TransformNoScale& transform)
{
	m_transform = transform;
	CacheViewProjMatrices();
}

void Camera::CacheViewProjMatrices()
{
	auto posVec = m_transform.GetPosition();
	auto translateMat = XMMatrixTranslation(-posVec.x, -posVec.y, -posVec.z);
	auto viewMat = translateMat * m_transform.GetRotationQuat().GetMatrixFormXM();
	
	XMStoreFloat4x4(&m_viewMatrix, viewMat);
	XMStoreFloat4x4(&m_viewProjMatrix, viewMat * m_projMatrix.GetMatrixXM());
}