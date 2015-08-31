#include "PCH.h"
#include "CameraController.h"
#include "GameStateEvents.h"


CameraController::CameraController(TransformNoScale* pCamTransform)
	: m_pCamTransform(pCamTransform)
	, m_keys{'W', 'A', 'S', 'D', 'J', 'L', 'I', 'K', 'Q', 'E', 'T', 'G'}
{
	RegisterForEvent("KeyUp"_HASH);
	RegisterForEvent("KeyDown"_HASH);
	RegisterForEvent("MouseMove"_HASH);
	RegisterForEvent("RMouseDown"_HASH);
	RegisterForEvent("RMouseUp"_HASH);
	RegisterForEvent("Tick"_HASH);
	Init();
}

void CameraController::Update(F32 dt)
{
	float lDistance = 10.0f * dt;
	float rDistance = 1.0f * dt;
	auto& rotationQuat = m_pCamTransform->GetRotationQuat();

	if (m_bPressed['W'])
		m_pCamTransform->Translate(rotationQuat.GetForwardVectorXM() * lDistance);

	if (m_bPressed['S'])
		m_pCamTransform->Translate(rotationQuat.GetForwardVectorXM() * -lDistance);

	if (m_bPressed['D'])
		m_pCamTransform->Translate(rotationQuat.GetRightVectorXM() * lDistance);
	
	if (m_bPressed['A'])
		m_pCamTransform->Translate(rotationQuat.GetRightVectorXM() * -lDistance);

	if (m_bPressed['T'])
		m_pCamTransform->Translate(rotationQuat.GetUpVectorXM() * lDistance * 0.5f);

	if (m_bPressed['G'])
		m_pCamTransform->Translate(rotationQuat.GetUpVectorXM() * -lDistance * 0.5f);

	// Roll 
	if (m_bPressed['Q'])
		m_pCamTransform->RotateZ(-rDistance);

	if (m_bPressed['E'])
		m_pCamTransform->RotateZ(rDistance);

	// Keyboard rotation
	if (m_bPressed['I'])
		m_pCamTransform->RotateX(rDistance);

	if (m_bPressed['K'])
		m_pCamTransform->RotateX(-rDistance);

	if (m_bPressed['J'])
		m_pCamTransform->RotateY(rDistance);

	if (m_bPressed['L'])
		m_pCamTransform->RotateY(-rDistance);

	// Mouse rotation
	if (m_bRmbDown)
	{
		float sensitivity = .1f;
		auto reX = m_mouseDeltaY * sensitivity * dt;
		auto reY = m_mouseDeltaX * sensitivity * dt;

		if (reX != 0.0f)
		{
			m_pCamTransform->RotateX(reX);
		}
		
		if (reY != 0.0f)
		{
			m_pCamTransform->RotateY(reY);
		}

		m_mouseDeltaX = 0;
		m_mouseDeltaY = 0;
	}
}

void CameraController::Init()
{
	XMFLOAT3 vFocalPoint(0.0f, 0.0f, 0.0f);
	XMVECTOR focalPoint = XMLoadFloat3(&vFocalPoint);

	XMFLOAT3 vUp(0.0f, 1.0f, 0.0f);
	XMVECTOR up = XMLoadFloat3(&vUp);

	XMFLOAT3 vPosition(0.0f, 0.0f, -3.0f);
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
		Update(ev.data[0].asFloat);
		break;
	case "KeyUp"_HASH:
		for (auto key : m_keys)
			if (ev.data[0].asU32 == key)
			{
				m_bPressed[key] = false;
				break;
			}
		break;
	case "KeyDown"_HASH:
		for (auto key : m_keys)
			if (ev.data[0].asU32 == key)
			{
				m_bPressed[key] = true;
				break;
			}
		break;
	case "MouseMove"_HASH:
		if (m_bRmbDown)
		{
			if (!(m_oldMouseX == 0 && m_oldMouseY == 0))
			{
				m_mouseDeltaX = m_oldMouseX - ev.data[0].asU32;
				m_mouseDeltaY = m_oldMouseY - ev.data[1].asU32;
			}
			m_oldMouseX = ev.data[0].asU32;
			m_oldMouseY = ev.data[1].asU32;
		}
		break;
	case "RMouseDown"_HASH:
		m_oldMouseX = ev.data[0].asU32;
		m_oldMouseY = ev.data[1].asU32;
		m_mouseDeltaX = 0;
		m_mouseDeltaY = 0;
		m_bRmbDown = true;
		ShowCursor(FALSE);
		break;
	case "RMouseUp"_HASH:
		m_bRmbDown = false;
		ShowCursor(TRUE);
		break;
	}
}