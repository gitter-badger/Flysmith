#include "PCH.h"
#include "CameraController.h"


CameraController::CameraController(TransformNoScale* pCamTransform)
	: m_pCamTransform(pCamTransform)
	, m_keys{'W', 'A', 'S', 'D', 'J', 'L', 'I', 'K', 'Q', 'E', 'T', 'G'}
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

	if (m_bPressed['W'])
		m_pCamTransform->Translate(GetLookAt() * lDistance);

	if (m_bPressed['S'])
		m_pCamTransform->Translate(-GetLookAt() * lDistance);

	if (m_bPressed['A'])
		m_pCamTransform->Translate(-GetRight() * lDistance);

	if (m_bPressed['D'])
		m_pCamTransform->Translate(GetRight() * lDistance);

	if (m_bPressed['T'])
		m_pCamTransform->Translate(GetUp() * lDistance);

	if (m_bPressed['G'])
		m_pCamTransform->Translate(GetUp() * -lDistance);

	if (m_bPressed['I'])
		m_pCamTransform->RotateX(rDistance);

	if (m_bPressed['K'])
		m_pCamTransform->RotateX(-rDistance);

	if (m_bPressed['J'])
		m_pCamTransform->RotateY(rDistance);

	if (m_bPressed['L'])
		m_pCamTransform->RotateY(-rDistance);

	if (m_bPressed['Q'])
		m_pCamTransform->RotateZ(-rDistance);

	if (m_bPressed['E'])
		m_pCamTransform->RotateZ(rDistance);
}

XMVECTOR CameraController::GetUp()
{
	auto rotMat = m_pCamTransform->GetRotationMatrix();
	XMFLOAT3 upVec{ rotMat(0, 1), rotMat(1, 1), rotMat(2, 1) };
	return XMLoadFloat3(&upVec);
}

XMVECTOR CameraController::GetLookAt()
{
	auto rotMat = m_pCamTransform->GetRotationMatrix();
	XMFLOAT3 lookAtVec{ rotMat(0, 2), rotMat(1, 2), rotMat(2, 2) };
	return XMLoadFloat3(&lookAtVec);
}

XMVECTOR CameraController::GetRight()
{
	auto rotMat = m_pCamTransform->GetRotationMatrix();
	XMFLOAT3 rightVec{ rotMat(0, 0), rotMat(1, 0), rotMat(2, 0) };
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

	m_pCamTransform->SetPosition(position);

	XMVECTOR lookAt = XMVector3Normalize(focalPoint - position);
	XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, lookAt));
	XMVECTOR upVec = XMVector3Normalize(XMVector3Cross(lookAt, right));

	SetAxis(lookAt, upVec, right);
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

	m_pCamTransform->SetRotation(rot);
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