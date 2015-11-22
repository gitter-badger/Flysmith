#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>
using namespace DirectX;


#pragma warning(push)
#pragma warning(disable:4251)
class KEBABCOMMON_API Quaternion
{
public:
	Quaternion();
	Quaternion(const Quaternion&);
	Quaternion& operator=(const Quaternion&);

	explicit Quaternion(CXMMATRIX mat);
	explicit Quaternion(const XMFLOAT4X4& mat);
	explicit Quaternion(const XMFLOAT3& axis, float angle);
	explicit Quaternion(const XMFLOAT4& quaternion);
	explicit Quaternion(float pitch, float yaw, float roll);
	explicit Quaternion(CXMVECTOR quaternion);

	void SetFromEuler(float pitch, float yaw, float roll);
	void SetFromEuler(const XMFLOAT3&);
	void SetFromMatrix(CXMMATRIX mat);
	void SetFromMatrix(const XMFLOAT4X4& mat);

	XMFLOAT4X4 GetMatrixForm() const;
	XMMATRIX GetMatrixFormXM() const;

	// X Axis
	XMVECTOR GetRightVectorXM() const;

	// Y Axis
	XMVECTOR GetUpVectorXM() const;

	// Z Axis
	XMVECTOR GetForwardVectorXM() const;

	void Concat(const XMFLOAT3& axis, float angle);
	void Concat(const XMFLOAT4& quaternion);
	void Concat(CXMVECTOR quaternion);
	void Concat(const Quaternion& other);
	Quaternion operator*=(const Quaternion&);
	Quaternion operator*(const Quaternion&);

private:
	XMFLOAT4 quat;
};

const XMFLOAT3 xAxis = { 1.0f, 0.0f, 0.0f },
yAxis = { 0.0f, 1.0f, 0.0f },
zAxis = { 0.0f, 0.0f, 1.0f };

#pragma warning(pop)