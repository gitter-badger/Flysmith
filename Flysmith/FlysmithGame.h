#pragma once
#include "Application.h"
#include "CameraController.h"
#include "EventListener.h"
#include "Resources\ResourceRegistry.h"


class FlysmithGame : public Application, public EventListener
{
public:
	explicit FlysmithGame(HINSTANCE);
	void HandleEvent(const Event&) override;

protected:
	// dt is in seconds
	void UpdateScene(float dt) override;

private:
	void LoadResources();
	CameraController m_camController;
	ResourceRegistry m_resources;
};