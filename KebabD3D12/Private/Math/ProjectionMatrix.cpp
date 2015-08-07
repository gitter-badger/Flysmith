#include "PCH.h"
#include "ProjectionMatrix.h"


XMMATRIX cuc::ProjectionMatrix::GetMatrixXM()
{
	return XMLoadFloat4x4(&m_projMatrix);
}

XMFLOAT4X4 cuc::ProjectionMatrix::GetMatrix()
{
	return m_projMatrix;
}

cuc::ProjectionMatrix::ProjectionMatrix()
{
}
