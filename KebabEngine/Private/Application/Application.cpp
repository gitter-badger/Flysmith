#include "PCH.h"
#include "Application.h"
#include "Events\EventManager.h"
#include "Window.h"
using namespace cuc;


EventManager cuc::g_eventManager;
#include "Resources\Font.h"
Application::Application(HINSTANCE hInstance)
{
	m_pWindow = std::make_shared<Window>(hInstance, 800, 600, L"Kebab Engine", false);
	Font font;
	font.LoadFromFile("Aller_Rg.ttf");
}

Application::~Application()
{
}

#ifdef RENDERER_DX12
#include "../../KebabD3D12/Public/Renderer.h"
#elif defined(RENDERER_OGL)
#include "../KebabOGL/GLRenderer.h"
#endif

U32 Application::Run()
{
#ifdef RENDERER_DX12
	Renderer renderer(m_pWindow->GetHandle(), m_pWindow->GetWidth(), m_pWindow->GetHeight());
#elif defined(RENDERER_OGL)
	GLRenderer renderer(m_pWindow->GetHandle(), m_pWindow->GetHeight(), m_pWindow->GetHeight());
#endif
	
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