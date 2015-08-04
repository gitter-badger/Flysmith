#pragma once
#include "PublicDef.h"


namespace cuc
{
	class Camera
	{
	public:
		Camera(DirectX::XMFLOAT3 position);
		~Camera();
		DirectX::XMMATRIX GetViewMatrix();
		DirectX::XMMATRIX GetProjMatrix(float fov, float aspectRatio, float nearPlane = 1.0f, float farPlane = 1000.0f);
		
		void SetPosition(const DirectX::XMFLOAT3&);

	private:
		struct Impl;
		Impl* m_pImpl;
	};
}