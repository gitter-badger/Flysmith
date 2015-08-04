#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>
using namespace DirectX;


#pragma warning(push)
#pragma warning(disable:4251)
namespace cuc
{
	class KEBAB12_API Transform
	{
	public:
		Transform();
		Transform(const XMFLOAT3& position);
		Transform(const XMFLOAT3& position, const XMFLOAT3& rotation);
		Transform(const XMFLOAT3& position, float pitch, float yaw, float roll);
		Transform(const XMFLOAT3& position, const XMFLOAT3& rotation, const XMFLOAT3& scale);

		const XMFLOAT4X4& GetMatrix() const;
		const XMMATRIX GetMatrixXM() const;

		const XMFLOAT3& GetPosition() const;
		const XMVECTOR GetPositionXM() const;
		
		const XMFLOAT3& GetRotation() const;
		const XMVECTOR GetRotationXM() const;

		const XMFLOAT3& GetScale() const;
		const XMVECTOR GetScaleXM() const;

		// TODO: Transform concatenation

		friend bool operator==(const Transform&, const Transform&);
		friend bool operator!=(const Transform&, const Transform&);

		// Absolute values
		void SetPosition(const XMFLOAT3&);
		void SetPosition(float x, float y, float z);
		
		void SetRotation(const XMFLOAT3&);
		void SetRotation(float pitchAngle, float yawAngle, float rollAngle);
		
		void SetScale(float uniformScale);
		void SetScale(const XMFLOAT3&);
		void SetScale(float x, float y, float z);

		// Relative values
		void Translate(const XMFLOAT3&);
		void Translate(float dX, float dY, float dZ);
		void TranslateX(float dX);
		void TranslateY(float dY);
		void TranslateZ(float dZ);

		void Rotate(const XMFLOAT3&);
		void Rotate(float dPitchAngle, float dYawAngle, float dRollAngle);
		void RotateX(float dPitchAngle);
		void RotateY(float dYawAngle);
		void RotateZ(float dRollAngle);

		void Scale(float byUniformScale);
		void Scale(const XMFLOAT3&);
		void Scale(float byX, float byY, float byZ);
		void ScaleX(float byX);
		void ScaleY(float byY);
		void ScaleZ(float byZ);

	private:
		void CacheTransform();
		XMFLOAT4X4 m_transformMatrix;

		XMFLOAT3 m_position;
		XMFLOAT3 m_rotation;
		XMFLOAT3 m_scale;
	};
}
#pragma warning(pop)