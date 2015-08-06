#pragma once
#include "Application.h"
#include "CameraController.h"
#include "EventListener.h"


class FlysmithGame : public cuc::Application, public EventListener
{
public:
	FlysmithGame(HINSTANCE);
	void HandleEvent(const Event&);

protected:
	void UpdateScene(float dt);

private:
	void GenerateFoilMesh(const std::wstring&);
	CameraController m_camController;
};