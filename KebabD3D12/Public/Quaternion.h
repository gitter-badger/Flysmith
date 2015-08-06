#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>
using namespace DirectX;


namespace cuc
{
	class KEBAB12_API Quaternion
	{
	public:
		Quaternion();
		explicit Quaternion(CXMMATRIX mat);
		explicit Quaternion(const XMFLOAT4X4& mat);
		explicit Quaternion(const XMFLOAT3& axis, float angle);
		explicit Quaternion(const XMFLOAT4& quaternion);
		explicit Quaternion(CXMVECTOR quaternion);

		void SetFromMatrix(CXMMATRIX mat);
		void SetFromMatrix(const XMFLOAT4X4& mat);
		
		XMFLOAT4X4 GetMatrix() const;
		XMMATRIX GetMatrixXM() const;

		void Concat(const XMFLOAT3& axis, float angle);
		void Concat(const XMFLOAT4& quaternion);
		void Concat(CXMVECTOR quaternion);
		void Concat(const Quaternion& other);
		Quaternion operator*=(const Quaternion&);

	private:
		XMFLOAT4 quat;
	};
}