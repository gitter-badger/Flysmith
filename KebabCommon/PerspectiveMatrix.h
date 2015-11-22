#pragma once
#include "PublicDef.h"
#include "ProjectionMatrix.h"
#include <DirectXMath.h>
using namespace DirectX;


class KEBABCOMMON_API PerspectiveMatrix : public ProjectionMatrix
{
public:
	PerspectiveMatrix(float width, float height, float fovY, float nearPlane = 1.0f, float farPlane = 1000.0f);
};