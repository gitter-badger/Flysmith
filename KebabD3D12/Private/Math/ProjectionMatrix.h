#pragma once
#include <DirectXMath.h>
using namespace DirectX;

namespace cuc
{
	class ProjectionMatrix
	{
	public:
		XMMATRIX GetMatrixXM();
		XMFLOAT4X4 GetMatrix();

	protected:
		XMFLOAT4X4 m_projMatrix;
		ProjectionMatrix();
	};
}