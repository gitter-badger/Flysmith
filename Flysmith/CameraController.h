#pragma once
#include "EventListener.h"
#include "../../KebabD3D12/Public/TransformNoScale.h"



class CameraController : public EventListener
{
public:
	CameraController(TransformNoScale* pCamTransform);
	void Update(F32 dt);
	void HandleEvent(const Event& ev);

private:
	void Init();
	void SetAxis(CXMVECTOR lookAt, CXMVECTOR up, CXMVECTOR right);

	TransformNoScale* m_pCamTransform;
};