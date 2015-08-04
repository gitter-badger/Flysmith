#pragma once
#include "Application.h"
#include "CameraController.h"


class FlysmithGame : public cuc::Application
{
public:
	FlysmithGame(HINSTANCE);

protected:
	void UpdateScene();

private:
	CameraController m_camController;
};