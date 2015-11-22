#include "PCH.h"
#include "Renderer.h"
#include "Dependencies\GLCore.h"
#include "GLContext.h"


#pragma comment(lib, "opengl32.lib")


struct Renderer::Impl
{
	Impl(HWND windowHandle, const U32 windowWidth, const U32 windowHeight);

	GLContext m_context;
};

Renderer::Renderer(HWND windowHandle, const U32 windowWidth, const U32 windowHeight)
{
	m_pImpl = new Impl(windowHandle, windowWidth, windowHeight);
}

Renderer::~Renderer()
{
	delete m_pImpl;
}

void Renderer::Render() const
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
	m_pImpl->m_context.SwapContextBuffers();
}

Renderer::Impl::Impl(HWND windowHandle, const U32 windowWidth, const U32 windowHeight)
	: m_context(windowHandle, windowWidth, windowHeight)
{
}
