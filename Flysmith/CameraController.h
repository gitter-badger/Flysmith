#pragma once
#include "EventListener.h"
#include "GameStateEvents.h"
#include "../../KebabD3D12/Public/Transform.h"
#include "../../KebabD3D12/Public/Quaternion.h"
using namespace cuc;


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
};