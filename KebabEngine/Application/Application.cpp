#include "Application.h"
#include "Events\EventManager.h"
#include "Window.h"
using namespace cuc;


EventManager cuc::g_eventManager;

Application::Application(HINSTANCE hInstance)
	: m_pWindow(nullptr)
{
	m_pWindow = new Window(hInstance, 800, 600, L"Kebab", false);
}

Application::~Application()
{
	delete m_pWindow;
}

#include "Renderer.h"
U32 Application::Run()
{
	Renderer renderer;
	renderer.m_pContext = m_pWindow->GetContext();
	
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