#pragma once
#include "ProjectionMatrix.h"
#include <DirectXMath.h>
using namespace DirectX;


class PerspectiveMatrix : public ProjectionMatrix
{
public:
	PerspectiveMatrix(float width, float height, float fovY, float nearPlane = 1.0f, float farPlane = 1000.0f);
};