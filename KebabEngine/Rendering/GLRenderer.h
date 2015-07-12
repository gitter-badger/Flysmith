#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_OGL
#include "Events\EventListener.h"
#include "OpenGL\GLContext.h"
#include <memory>


namespace cuc
{
	class Window;

	class GLRenderer : public EventListener
	{
	public:
		GLRenderer(const std::shared_ptr<Window>&);
		~GLRenderer();
		void HandleEvent(const Event&);
		void Render();

	private:
		std::shared_ptr<Window> m_pWindow;
		GLContext m_context;
	};
}
#endif // RENDERER_OGL