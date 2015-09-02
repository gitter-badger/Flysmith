#include "PCH.h"
#include "Application.h"
#include "GameStateEvents.h"
#include "Events\EventManager.h"
#include "InputManager.h"
#include "Window.h"


EventManager g_eventManager;
InputManager g_inputManager;

Application::Application(HINSTANCE hInstance)
{
	m_pWindow = std::make_shared<Window>(hInstance, 800, 600, L"Kebab Engine", false);
	m_pRenderer = new Renderer(m_pWindow->GetHandle(), m_pWindow->GetWidth(), m_pWindow->GetHeight());
	g_inputManager.Init(m_pWindow.get());
}

Application::~Application()
{
	delete m_pRenderer;
}

U32 Application::Run()
{
	m_timer.Reset();
	
	HWND hwnd = m_pWindow->GetHandle();
	bool bWindowActive = true;
	m_pWindow->Clip();
	while (!m_pWindow->ShouldClose())
	{
		HWND activeWindow = GetForegroundWindow();
		if (activeWindow != hwnd)
		{
			g_inputManager.EnableCursor();
			g_inputManager.SetRMBUp();
			g_inputManager.SetLMBUp();
			m_pWindow->Unclip();
			bWindowActive = false;
		}
		else
		{
			if (!bWindowActive)
			{
				g_inputManager.EnableCursor();
				m_pWindow->Clip();
				bWindowActive = true;
			}
		}

		g_inputManager.Reset();

		m_pWindow->RunMessageLoop();
		
		m_timer.Update();
		g_eventManager.PostEvent(CreateTickEvent(m_timer.GetElapsed()));
		g_eventManager.DispatchEvents();
		UpdateScene(m_timer.GetElapsed());

		CopyRenderData();
		m_pRenderer->Render();
	}

	return 0;
}

void Application::CopyRenderData()
{
	m_pRenderer->UpdateScene(m_scene.m_renderComponents);
	m_pRenderer->UpdateView(m_scene.m_camTransform);
}
