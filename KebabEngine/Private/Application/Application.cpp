#include "PCH.h"
#include "Application.h"
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
	
	Transform tempObjTransform;
	Transform tempCamTransform;

	while (!m_pWindow->ShouldClose())
	{
		m_pWindow->RunMessageLoop();
		
		m_timer.Update();

		g_eventManager.DispatchEvents();

		// Do stuff with tempObjTransform
		tempObjTransform.TranslateX(.0001f);
		// Do stuff with tempCamTransform

		// Copy scene data to renderer
		m_pRenderer->UpdateView(tempCamTransform);
		m_pRenderer->UpdateScene(tempObjTransform);

		m_pRenderer->Render();
	}

	return 0;
}