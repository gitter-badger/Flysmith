#pragma once
#include "ProjectionMatrix.h"
#include <DirectXMath.h>
using namespace DirectX;


namespace cuc
{
	class OrthographicMatrix : public ProjectionMatrix
	{
	public:
		OrthographicMatrix(float width, float height, float nearPlane = 0.0f, float farPlane = 100.0f);
	};
}