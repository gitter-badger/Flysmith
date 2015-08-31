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
	std::map<char, bool> m_bPressed;
	std::vector<unsigned char> m_keys;

	I32 m_oldMouseX = 0;
	I32 m_oldMouseY = 0;
	I32 m_mouseDeltaX = 0;
	I32 m_mouseDeltaY = 0;
	bool m_bRmbDown = false;
};