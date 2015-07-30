#pragma once
#include "PublicDef.h"
#include "Timer.h"

#ifdef RENDERER_DX12
#include "../../KebabD3D12/Public/Renderer.h"
#elif defined(RENDERER_OGL)
#include "../../KebabOGL/Public/Renderer.h"
#endif


namespace cuc
{
	class Window;

	class KEBAB_API Application
	{
	public:
		Application(HINSTANCE);
		virtual ~Application();

		virtual U32 Run();

	protected:
		Renderer* m_pRenderer;

	private:
		Timer m_timer;
		std::shared_ptr<Window> m_pWindow;
	};
}