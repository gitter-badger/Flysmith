#include "PCH.h"
#include "Camera.h"
#include "Transform.h"
using namespace DirectX;
using namespace cuc;


struct Camera::Impl
{
	Impl(XMFLOAT3 position);

	Transform m_transform;
	XMFLOAT3 m_lookDirection;
	XMFLOAT3 m_upDirection;
};

Camera::Impl::Impl(XMFLOAT3 position)
	: m_transform(position, 0.0f, 0.0f, 0.0f)
	, m_lookDirection(0.0f, 0.0f, -1.0f)
	, m_upDirection(0.0f, 1.0f, 0.0f)
{
}

Camera::Camera(XMFLOAT3 position)
{
	m_pImpl = new Impl(position);
}

Camera::~Camera()
{
	delete m_pImpl;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return XMMatrixLookToRH(XMLoadFloat3(&m_pImpl->m_transform.GetPosition()),
							XMLoadFloat3(&m_pImpl->m_lookDirection),
							XMLoadFloat3(&m_pImpl->m_upDirection));
}

XMMATRIX cuc::Camera::GetProjMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	return XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
}

void cuc::Camera::SetPosition(const DirectX::XMFLOAT3& newPosition)
{
	m_pImpl->m_transform.SetPosition(newPosition);
}
