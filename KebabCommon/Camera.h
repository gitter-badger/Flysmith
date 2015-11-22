#pragma once
#include "TransformNoScale.h"
#include "PerspectiveMatrix.h"
using namespace DirectX;


class KEBABCOMMON_API Camera
{
public:
	Camera(float viewWidth, float viewHight, float fovY = 0.8f, float nearPlane = 1.0f, float farPlane = 1000.0f);
	~Camera();

	const XMFLOAT4X4& GetViewMatrix() const;
	const XMMATRIX GetViewMatrixXM() const;

	const XMFLOAT4X4& GetViewProjMatrix() const;
	const XMMATRIX GetViewProjMatrixXM() const;

	void Update(const TransformNoScale&);

private:
	void CacheViewProjMatrices();

	TransformNoScale m_transform;
	PerspectiveMatrix m_projMatrix;
	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_viewProjMatrix;
};