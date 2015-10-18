#pragma once
#include "Application.h"
#include "CameraController.h"
#include "EventListener.h"
#include "Resources\ResourceRegistry.h"


class FlysmithGame : public Application, public EventListener
{
public:
	FlysmithGame(HINSTANCE);
	void HandleEvent(const Event&);

protected:
	// dt is in seconds
	void UpdateScene(float dt);

private:
	void LoadResources();
	CameraController m_camController;
	ResourceRegistry m_resources;
};