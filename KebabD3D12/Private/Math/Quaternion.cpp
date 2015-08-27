#include "PCH.h"
#include "Quaternion.h"


Quaternion::Quaternion()
{
	XMStoreFloat4(&quat, XMQuaternionIdentity());
}

Quaternion::Quaternion(const Quaternion& other)
	: quat(other.quat)
{
}

Quaternion& Quaternion::operator=(const Quaternion& other)
{
	quat = other.quat;
	return *this;
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

Quaternion::Quaternion(float pitch, float yaw, float roll)
{
	SetFromEuler(pitch, yaw, roll);
}

Quaternion::Quaternion(CXMVECTOR quaternion)
{
	XMStoreFloat4(&quat, quaternion);
}

void Quaternion::SetFromEuler(float pitch, float yaw, float roll)
{
	*this = Quaternion(xAxis, pitch) * Quaternion(yAxis, yaw) * Quaternion(zAxis, roll);
}

void Quaternion::SetFromEuler(const XMFLOAT3& vec)
{
	SetFromEuler(vec.x, vec.y, vec.z);
}

void Quaternion::SetFromMatrix(CXMMATRIX mat)
{
	XMStoreFloat4(&quat, XMQuaternionRotationMatrix(mat));
}

void Quaternion::SetFromMatrix(const XMFLOAT4X4& mat)
{
	SetFromMatrix(XMLoadFloat4x4(&mat));
}

XMFLOAT4X4 Quaternion::GetMatrixForm() const
{
	XMFLOAT4X4 rotMatrix;
	XMStoreFloat4x4(&rotMatrix, XMMatrixRotationQuaternion(XMLoadFloat4(&quat)));
	return rotMatrix;
}

XMMATRIX Quaternion::GetMatrixFormXM() const
{
	return XMMatrixRotationQuaternion(XMLoadFloat4(&quat));
}

XMVECTOR Quaternion::GetUpVectorXM() const
{
	auto rotMat = GetMatrixForm();
	XMFLOAT3 up(rotMat(0, 1), rotMat(1, 1), rotMat(2, 1));
	return XMLoadFloat3(&up);
}

XMVECTOR Quaternion::GetForwardVectorXM() const
{
	auto rotMat = GetMatrixForm();
	XMFLOAT3 forward(rotMat(0, 2), rotMat(1, 2), rotMat(2, 2));
	return XMLoadFloat3(&forward);
}

XMVECTOR Quaternion::GetRightVectorXM() const
{
	auto rotMat = GetMatrixForm();
	XMFLOAT3 right(rotMat(0, 0), rotMat(1, 0), rotMat(2, 0));
	return XMLoadFloat3(&right);
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

Quaternion Quaternion::operator*(const Quaternion& other)
{
	return Quaternion(XMQuaternionMultiply(XMLoadFloat4(&quat), XMLoadFloat4(&other.quat)));
}
