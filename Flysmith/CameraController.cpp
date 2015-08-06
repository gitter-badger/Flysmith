#include "PCH.h"
#include "CameraController.h"


CameraController::CameraController(XMFLOAT3* camPosition, Quaternion* camRotation)
	: m_camPosition(camPosition)
	, m_camRotation(camRotation)
	, m_keys{'W', 'A', 'S', 'D', 'J', 'L', 'I', 'K', 'Q', 'E'}
{
	RegisterForEvent(KeyboardEvent::KeyUpId);
	RegisterForEvent(KeyboardEvent::KeyDownId);
	RegisterForEvent("Tick"_HASH);
	Init();
}

void CameraController::Update(F32 dt)
{
	float lDistance = 10.0f * dt;
	float rDistance = 1.0f * dt;

	static XMFLOAT3 xAxis(1.0f, 0.0f, 0.0f),
					yAxis(0.0f, 1.0f, 0.0f),
					zAxis(0.0f, 0.0f, 1.0f);

	if (m_bPressed['W'])
		Move(GetLookAt() * lDistance);

	if (m_bPressed['S'])
		Move(-GetLookAt() * lDistance);

	if (m_bPressed['A'])
		Move(-GetRight() * lDistance);

	if (m_bPressed['D'])
		Move(GetRight() * lDistance);

	if (m_bPressed['I'])
		m_camRotation->Concat(xAxis, rDistance);

	if (m_bPressed['K'])
		m_camRotation->Concat(xAxis, -rDistance);

	if (m_bPressed['J'])
		m_camRotation->Concat(yAxis, rDistance);

	if (m_bPressed['L'])
		m_camRotation->Concat(yAxis, -rDistance);

	if (m_bPressed['Q'])
		m_camRotation->Concat(zAxis, -rDistance);

	if (m_bPressed['E'])
		m_camRotation->Concat(zAxis, rDistance);
}

XMVECTOR CameraController::GetLookAt()
{
	XMFLOAT4X4 mRot;
	XMStoreFloat4x4(&mRot, m_camRotation->GetMatrixXM());
	XMFLOAT3 lookAtVec{ mRot(0, 2), mRot(1, 2), mRot(2, 2) };
	return XMLoadFloat3(&lookAtVec);
}

XMVECTOR CameraController::GetRight()
{
	XMFLOAT4X4 mRot;
	XMStoreFloat4x4(&mRot, m_camRotation->GetMatrixXM());
	XMFLOAT3 rightVec{ mRot(0, 0), mRot(1, 0), mRot(2, 0) };
	return XMLoadFloat3(&rightVec);
}

void CameraController::Init()
{
	XMFLOAT3 vFocalPoint(0.0f, 0.0f, 0.0f);
	XMVECTOR focalPoint = XMLoadFloat3(&vFocalPoint);

	XMFLOAT3 vUp(0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMLoadFloat3(&vUp);

	XMFLOAT3 vPosition(0.0f, 0.0f, -20.0f);
	XMVECTOR position = XMLoadFloat3(&vPosition);

	XMStoreFloat3(m_camPosition, position);

	XMVECTOR lookAt = XMVector3Normalize(focalPoint - position);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, lookAt));
	XMVECTOR upVec = XMVector3Normalize(XMVector3Cross(lookAt, right));

	SetAxis(lookAt, upVec, right);
}

void CameraController::Move(CXMVECTOR disp)
{
	XMStoreFloat3(m_camPosition, XMLoadFloat3(m_camPosition) + disp);
}

void CameraController::SetAxis(CXMVECTOR lookAt, CXMVECTOR up, CXMVECTOR right)
{
	XMFLOAT4X4 rot;
	XMStoreFloat4x4(&rot, XMMatrixIdentity());

	XMFLOAT3 lookAtVec, upVec, rightVec;
	XMStoreFloat3(&lookAtVec, lookAt);
	XMStoreFloat3(&upVec, up);
	XMStoreFloat3(&rightVec, right);

	rot(0, 0) = rightVec.x;
	rot(0, 1) = upVec.x;
	rot(0, 2) = lookAtVec.x;

	rot(1, 0) = rightVec.y;
	rot(1, 1) = upVec.y;
	rot(1, 2) = lookAtVec.y;

	rot(2, 0) = rightVec.z;
	rot(2, 1) = upVec.z;
	rot(2, 2) = lookAtVec.z;

	m_camRotation->SetFromMatrix(rot);
}

void CameraController::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	case "Tick"_HASH:
		Update(ev[0].GetFloat());
		break;
	case KeyboardEvent::KeyUpId:
		for (auto key : m_keys)
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == key)
			{
				m_bPressed[key] = false;
				break;
			}
		break;
	case KeyboardEvent::KeyDownId:
		for (auto key : m_keys)
			if (ev[KeyboardEvent::U32_KEY_CODE].GetUnsignedInt() == key)
			{
				m_bPressed[key] = true;
				break;
			}
		break;
	}
}