#pragma once
#include "Application.h"
#include "CameraController.h"
#include "EventListener.h"
#include "ResourceRegistry.h"


class FlysmithGame : public cuc::Application, public EventListener
{
public:
	FlysmithGame(HINSTANCE);
	void HandleEvent(const Event&);

protected:
	void UpdateScene(float dt);

private:
	void LoadResources();
	CameraController m_camController;
	ResourceRegistry m_resources;
};