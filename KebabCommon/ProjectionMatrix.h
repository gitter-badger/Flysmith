#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>
using namespace DirectX;


class KEBABCOMMON_API ProjectionMatrix
{
public:
	XMMATRIX GetMatrixXM();
	XMFLOAT4X4 GetMatrix();

protected:
	XMFLOAT4X4 m_projMatrix;
	ProjectionMatrix();
};