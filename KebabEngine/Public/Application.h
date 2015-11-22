#pragma once
#include "PublicDef.h"
#include "Timer.h"
#include "Scene.h"

#ifdef RENDERER_DX12
#include "../../KebabD3D12/Public/Renderer.h"
#elif defined(RENDERER_GL)
#include "../../KebabGL/Public/Renderer.h"
#endif


class Window;

class KEBAB_API Application
{
public:
	Application(HINSTANCE);
	virtual ~Application();

	virtual U32 Run();

protected:
	// dt is in seconds
	virtual void UpdateScene(float dt) = 0;
	Scene m_scene;
	Timer m_timer;
	Renderer* m_pRenderer;

private:
	void CopyRenderData();
	std::shared_ptr<Window> m_pWindow;
};