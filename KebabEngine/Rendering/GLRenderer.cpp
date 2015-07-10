#include "GLRenderer.h"
#ifdef RENDERER_OGL
#include "GLCore.h"
#include "Events\WindowEvents.h"
#include "Application\Window.h"
using namespace cuc;


GLRenderer::GLRenderer(const std::shared_ptr<Window>& pWindow)
	: m_pWindow(pWindow)
	, m_context(pWindow->GetHandle(), pWindow->GetWidth(), pWindow->GetHeight())
{
	RegisterForEvent("WindowResize"_HASH);
	pWindow->GetHandle();
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::HandleEvent(const Event& event)
{
	switch (event.type)
	{
	case "WindowResize"_HASH:
		m_context.Resize(event[WindowResizeEvent::U32_WIDTH].GetUnsignedInt(), 
						 event[WindowResizeEvent::U32_HEIGHT].GetUnsignedInt());
		break;
	}
}

void GLRenderer::Render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
	m_context.SwapContextBuffers();
}
#endif