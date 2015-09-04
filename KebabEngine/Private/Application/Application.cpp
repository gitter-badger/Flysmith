#include "PCH.h"
#include "Application.h"
#include "GameStateEvents.h"
#include "Events\EventManager.h"
#include "InputManager.h"
#include "Window.h"
#include "../../KebabD3D12/Public/RenderItemProxy.h"


EventManager g_eventManager;
InputManager g_inputManager;

Application::Application(HINSTANCE hInstance)
{
	m_pWindow = std::make_shared<Window>(hInstance, 800, 600, L"Kebab Engine", false);
	m_pRenderer = new Renderer(m_pWindow->GetHandle(), m_pWindow->GetWidth(), m_pWindow->GetHeight());
	g_inputManager.Init(m_pWindow.get());
	m_scene.Init(m_pRenderer);
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
	std::vector<RenderItemProxy> visibleRenderables;
	// Hypothetical visibility culling
	for (auto& renderComponent : m_scene.renderComponents)
	{
		auto entityId = renderComponent.GetEntityId();
		// TODO: TEMP TEMP TEMP
		//auto entityId = 0;
		visibleRenderables.push_back(RenderItemProxy(m_scene.entities[entityId].GetWorldTransformMatrix(), renderComponent.GetRenderItem()));
	}

	m_pRenderer->UpdateScene(visibleRenderables);
	m_pRenderer->UpdateView(m_scene.camTransform);
}
