#include "OrthographicMatrix.h"


OrthographicMatrix::OrthographicMatrix(float width, float height, float nearPlane, float farPlane)
{
	XMStoreFloat4x4(&m_projMatrix, XMMatrixOrthographicLH(width, height, nearPlane, farPlane));
}
