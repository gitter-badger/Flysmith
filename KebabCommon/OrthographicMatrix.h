#pragma once
#include "PublicDef.h"
#include "ProjectionMatrix.h"
#include <DirectXMath.h>
using namespace DirectX;


class KEBABCOMMON_API OrthographicMatrix : public ProjectionMatrix
{
public:
	OrthographicMatrix(float width, float height, float nearPlane = 0.0f, float farPlane = 100.0f);
};