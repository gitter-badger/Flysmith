#pragma once
#include "PublicDef.h"
#include "Quaternion.h"
#include <DirectXMath.h>
using namespace DirectX;


#pragma warning(push)
#pragma warning(disable:4251)
namespace cuc
{
	class KEBAB12_API TransformNoScale
	{
	public:
		TransformNoScale();
		TransformNoScale(const XMFLOAT3& position);
		TransformNoScale(const XMFLOAT3& position, const Quaternion& rotation);
		TransformNoScale(const XMFLOAT3& position, float pitch, float yaw, float roll);

		const XMFLOAT4X4& GetMatrix() const;
		const XMMATRIX GetMatrixXM() const;

		const XMMATRIX GetRotationMatrixXM() const;

		const XMFLOAT3& GetPosition() const;
		const XMVECTOR GetPositionXM() const;

		// TODO: Transform concatenation

		friend bool operator==(const TransformNoScale&, const TransformNoScale&);
		friend bool operator!=(const TransformNoScale&, const TransformNoScale&);

		// Absolute values
		void SetPosition(CXMVECTOR);
		void SetPosition(const XMFLOAT3&);
		void SetPosition(float x, float y, float z);

		void SetRotation(CXMMATRIX);
		void SetRotation(const XMFLOAT4X4&);
		void SetRotation(const XMFLOAT3&);
		void SetRotation(float pitchAngle, float yawAngle, float rollAngle);
		void SetRotation(const Quaternion&);

		// Relative values
		void Translate(CXMVECTOR);
		void Translate(const XMFLOAT3&);
		void Translate(float dX, float dY, float dZ);
		void TranslateX(float dX);
		void TranslateY(float dY);
		void TranslateZ(float dZ);

		void Rotate(const Quaternion&);
		void RotateX(float dPitchAngle);
		void RotateY(float dYawAngle);
		void RotateZ(float dRollAngle);

	protected:
		virtual void CacheTransform();
		XMFLOAT4X4 m_transformMatrix;

		XMFLOAT3   m_position;
		Quaternion m_rotation;
	};
}
#pragma warning(pop)