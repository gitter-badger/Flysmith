#include "PCH.h"
#include "Camera.h"
using namespace DirectX;
using namespace cuc;


struct Camera::Impl
{
	Impl(XMFLOAT3 position);

	XMFLOAT3 m_position;
	F32 m_yaw;
	F32 m_pitch;
	XMFLOAT3 m_lookDirection;
	XMFLOAT3 m_upDirection;
};

Camera::Impl::Impl(XMFLOAT3 position)
	: m_position(position)
	, m_yaw(DirectX::XM_PI)
	, m_pitch(0.0f)
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
	return XMMatrixLookToRH(XMLoadFloat3(&m_pImpl->m_position),
							XMLoadFloat3(&m_pImpl->m_lookDirection),
							XMLoadFloat3(&m_pImpl->m_upDirection));
}

XMMATRIX cuc::Camera::GetProjMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	return XMMatrixPerspectiveFovRH(fov, aspectRatio, nearPlane, farPlane);
}
