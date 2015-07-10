#include "Application.h"
#include "Events\EventManager.h"
#include "Window.h"
using namespace cuc;


EventManager cuc::g_eventManager;

Application::Application(HINSTANCE hInstance)
	: m_pWindow(nullptr)
{
	m_pWindow = std::make_shared<Window>(hInstance, 800, 600, L"Kebab Engine", false);
}

Application::~Application()
{
}

#include "Rendering\GLRenderer.h"
U32 Application::Run()
{
	GLRenderer renderer(m_pWindow);
	
	m_timer.Reset();

	while (!m_pWindow->ShouldClose())
	{
		m_pWindow->RunMessageLoop();
		
		m_timer.Update();

		g_eventManager.DispatchEvents();

		renderer.Render();
	}

	return 0;
}