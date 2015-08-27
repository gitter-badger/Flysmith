#include "PCH.h"
#include "ProjectionMatrix.h"


XMMATRIX ProjectionMatrix::GetMatrixXM()
{
	return XMLoadFloat4x4(&m_projMatrix);
}

XMFLOAT4X4 ProjectionMatrix::GetMatrix()
{
	return m_projMatrix;
}

ProjectionMatrix::ProjectionMatrix()
{
}
