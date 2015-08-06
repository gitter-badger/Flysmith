#include "PCH.h"
#include "Quaternion.h"
using namespace cuc;


Quaternion::Quaternion()
{
	XMStoreFloat4(&quat, XMQuaternionIdentity());
}

Quaternion::Quaternion(CXMMATRIX mat)
{
	SetFromMatrix(mat);
}

Quaternion::Quaternion(const XMFLOAT4X4& mat)
{
	SetFromMatrix(mat);
}

Quaternion::Quaternion(const XMFLOAT3& axis, float angle)
{
	XMStoreFloat4(&quat, XMQuaternionRotationAxis(XMLoadFloat3(&axis), angle));
}

Quaternion::Quaternion(const XMFLOAT4& quaternion)
{
	XMStoreFloat4(&quat, XMLoadFloat4(&quaternion));
}

Quaternion::Quaternion(CXMVECTOR quaternion)
{
	XMStoreFloat4(&quat, quaternion);
}

void Quaternion::SetFromMatrix(CXMMATRIX mat)
{
	XMStoreFloat4(&quat, XMQuaternionRotationMatrix(mat));
}

void Quaternion::SetFromMatrix(const XMFLOAT4X4& mat)
{
	SetFromMatrix(XMLoadFloat4x4(&mat));
}

XMFLOAT4X4 Quaternion::GetMatrix() const
{
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, XMMatrixRotationQuaternion(XMLoadFloat4(&quat)));
	return rotMatrix;
}

XMMATRIX Quaternion::GetMatrixXM() const
{
	return XMMatrixRotationQuaternion(XMLoadFloat4(&quat));
}

void Quaternion::Concat(const XMFLOAT3& axis, float angle)
{
	Concat(Quaternion(axis, angle));
}

void Quaternion::Concat(const XMFLOAT4& quaternion)
{
	Concat(Quaternion(quaternion));
}

void Quaternion::Concat(CXMVECTOR quaternion)
{
	Concat(Quaternion(quaternion));
}

void Quaternion::Concat(const Quaternion& other)
{
	XMStoreFloat4(&quat, XMQuaternionMultiply(XMLoadFloat4(&quat), XMLoadFloat4(&other.quat)));
}

Quaternion Quaternion::operator*=(const Quaternion& other)
{
	Concat(other);
	return *this;
}