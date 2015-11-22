#pragma once
#include "EventListener.h"
#include "../../KebabCommon/TransformNoScale.h"



class CameraController : public EventListener
{
public:
	explicit CameraController(TransformNoScale* pCamTransform);
	void Update(F32 dt) const;
	void HandleEvent(const Event& ev) override;

private:
	void Init() const;
	void SetAxis(CXMVECTOR lookAt, CXMVECTOR up, CXMVECTOR right) const;

	TransformNoScale* m_pCamTransform;
};