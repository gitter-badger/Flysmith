#include "PCH.h"
#include "PerspectiveMatrix.h"


cuc::PerspectiveMatrix::PerspectiveMatrix(float width, float height, float fovY, float nearPlane, float farPlane)
{
	XMStoreFloat4x4(&m_projMatrix, XMMatrixPerspectiveFovLH(fovY, width / height, nearPlane, farPlane));
}
