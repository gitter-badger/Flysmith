#include "PCH.h"
#include "CameraController.h"
#include "GameStateEvents.h"
#include "InputManager.h"


CameraController::CameraController(TransformNoScale* pCamTransform)
	: m_pCamTransform(pCamTransform)
{
	RegisterForEvent("Tick"_HASH);
	RegisterForEvent("RMouseDown"_HASH);
	RegisterForEvent("RMouseUp"_HASH);
	Init();
}

void CameraController::Update(F32 dt)
{
	float lDistance = 10.0f * dt;
	float rDistance = 1.0f * dt;
	auto& rotationQuat = m_pCamTransform->GetRotationQuat();

	if (g_inputManager.IsKeyDown('W'))
		m_pCamTransform->Translate(rotationQuat.GetForwardVectorXM() * lDistance);

	if (g_inputManager.IsKeyDown('S'))
		m_pCamTransform->Translate(rotationQuat.GetForwardVectorXM() * -lDistance);

	if (g_inputManager.IsKeyDown('D'))
		m_pCamTransform->Translate(rotationQuat.GetRightVectorXM() * lDistance);
	
	if (g_inputManager.IsKeyDown('A'))
		m_pCamTransform->Translate(rotationQuat.GetRightVectorXM() * -lDistance);

	if (g_inputManager.IsKeyDown('T'))
		m_pCamTransform->Translate(rotationQuat.GetUpVectorXM() * lDistance * 0.25f);

	if (g_inputManager.IsKeyDown('G'))
		m_pCamTransform->Translate(rotationQuat.GetUpVectorXM() * -lDistance * 0.25f);

	// Roll 
	if (g_inputManager.IsKeyDown('Q'))
		m_pCamTransform->RotateZ(-rDistance);

	if (g_inputManager.IsKeyDown('E'))
		m_pCamTransform->RotateZ(rDistance);

	// Keyboard rotation
	if (g_inputManager.IsKeyDown('I'))
		m_pCamTransform->RotateX(rDistance);

	if (g_inputManager.IsKeyDown('K'))
		m_pCamTransform->RotateX(-rDistance);

	if (g_inputManager.IsKeyDown('J'))
		m_pCamTransform->RotateY(rDistance);

	if (g_inputManager.IsKeyDown('L'))
		m_pCamTransform->RotateY(-rDistance);

	// Mouse rotation
	if (g_inputManager.IsRMBDown())
	{
		float sensitivity = .1f;
		auto reX = -g_inputManager.GetMouseDeltaY() * sensitivity * dt;
		auto reY = -g_inputManager.GetMouseDeltaX() * sensitivity * dt;

		if (reX != 0.0f)
		{
			m_pCamTransform->RotateX(reX);
		}
		
		if (reY != 0.0f)
		{
			m_pCamTransform->RotateY(reY);
		}
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
	case "RMouseDown"_HASH:
		ShowCursor(FALSE);
		break;
	case "RMouseUp"_HASH:
		ShowCursor(TRUE);
		break;
	}
}