#include "PCH.h"
#include "RendererImpl.h"



Renderer::Impl::Impl(HWND windowHandle, const U32 windowWidth, const U32 windowHeight)
	: m_context(windowHandle, windowWidth, windowHeight)
	, m_projPerspective(800.0f, 600.0f, 45.0f)
	, m_camera(800.0f, 600.0f)
{
}