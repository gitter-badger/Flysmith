#include "PCH.h"
#include "Application.h"
#include "GameStateEvents.h"
#include "Events\EventManager.h"
#include "Window.h"
using namespace cuc;


EventManager cuc::g_eventManager;
Application::Application(HINSTANCE hInstance)
{
	m_pWindow = std::make_shared<Window>(hInstance, 800, 600, L"Kebab Engine", false);
	m_pRenderer = new Renderer(m_pWindow->GetHandle(), m_pWindow->GetWidth(), m_pWindow->GetHeight());
}

Application::~Application()
{
	delete m_pRenderer;
}

U32 Application::Run()
{
	m_timer.Reset();
	
	while (!m_pWindow->ShouldClose())
	{
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

void cuc::Application::CopyRenderData()
{
	m_pRenderer->UpdateScene(m_scene.m_renderComponents);
	m_pRenderer->UpdateView(m_scene.m_camTransform);
}
