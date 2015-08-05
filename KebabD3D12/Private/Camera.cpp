#include "PCH.h"
#include "Camera.h"
using namespace cuc;


Camera::Camera(float aspectRatio, float fov, float nearPlane, float farPlane)
	: m_position(0.0f, 0.0f, 0.0f)
	, m_aspectRatio(aspectRatio)
	, m_fieldOfView(fov)
	, m_nearPlane(nearPlane)
	, m_farPlane(farPlane)
{
	XMStoreFloat4(&m_rotation, XMQuaternionIdentity());
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

void cuc::Camera::Update(const XMFLOAT3& position, const XMFLOAT4& rotation)
{
	m_position = position;
	m_rotation = rotation;
	CacheViewProjMatrices();
}

void cuc::Camera::CacheViewProjMatrices()
{
	auto translate = XMMatrixTranslation(-m_position.x, -m_position.y, -m_position.z);
	
	auto viewMat = translate * XMMatrixRotationQuaternion(XMLoadFloat4(&m_rotation));
	//XMStoreFloat4x4(&m_viewMatrix, m_transform.GetMatrixXM() * XMLoadFloat4x4(&m_viewMatrix));
	
	// Rarely updated. To split.
	auto projMat = XMMatrixPerspectiveFovLH(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);
	
	XMStoreFloat4x4(&m_viewMatrix, viewMat);
	XMStoreFloat4x4(&m_projMatrix, projMat);
	XMStoreFloat4x4(&m_viewProjMatrix, viewMat * projMat);
}