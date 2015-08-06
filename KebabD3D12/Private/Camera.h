#pragma once
#include "PublicDef.h"
#include "Transform.h"
#include "Quaternion.h"
using namespace DirectX;


namespace cuc
{
	class Camera
	{
	public:
		Camera(float aspectRatio, float fov = 0.8f, float nearPlane = 1.0f, float farPlane = 1000.0f);
		~Camera();

		const XMFLOAT4X4& GetViewMatrix() const;
		const XMMATRIX GetViewMatrixXM() const;

		const XMFLOAT4X4& GetProjMatrix() const;
		const XMMATRIX GetProjMatrixXM() const;

		const XMFLOAT4X4& GetViewProjMatrix() const;
		const XMMATRIX GetViewProjMatrixXM() const;
		
		void Update(const TransformNoScale&);

	private:
		void CacheViewProjMatrices();

		float m_aspectRatio;
		float m_fieldOfView;
		float m_nearPlane;
		float m_farPlane;

		TransformNoScale m_transform;

		XMFLOAT4X4 m_projMatrix;
		XMFLOAT4X4 m_viewMatrix;
		XMFLOAT4X4 m_viewProjMatrix;
	};
}