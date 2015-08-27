#pragma once
#include "PublicDef.h"
#include "TransformNoScale.h"


#pragma warning(push)
#pragma warning(disable:4251)
class KEBAB12_API Transform : public TransformNoScale
{
public:
	Transform();
	Transform(const XMFLOAT3& position);
	Transform(const XMFLOAT3& position, const Quaternion& rotation);
	Transform(const XMFLOAT3& position, float pitch, float yaw, float roll);
	Transform(const XMFLOAT3& position, const Quaternion& rotation, const XMFLOAT3& scale);

	const XMFLOAT3& GetScale() const;
	const XMVECTOR GetScaleXM() const;

	// TODO: Transform concatenation

	friend bool operator==(const Transform&, const Transform&);
	friend bool operator!=(const Transform&, const Transform&);

	// Absolute values
	void SetScale(float uniformScale);
	void SetScale(const XMFLOAT3&);
	void SetScale(float x, float y, float z);

	// Relative values
	void Scale(float byUniformScale);
	void Scale(const XMFLOAT3&);
	void Scale(float byX, float byY, float byZ);
	void ScaleX(float byX);
	void ScaleY(float byY);
	void ScaleZ(float byZ);

protected:
	void CacheTransform();

private:
	XMFLOAT3 m_scale;
};
#pragma warning(pop)