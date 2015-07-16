#include "PCH.h"
#include "GLRenderer.h"
#include "Dependencies\GLCore.h"
using namespace cuc;


#pragma comment(lib, "opengl32.lib")

GLRenderer::GLRenderer(HWND windowHandle, const U32 windowWidth, const U32 windowHeight)
	: m_context(windowHandle, windowWidth, windowHeight)
{
}

GLRenderer::~GLRenderer()
{
}

void GLRenderer::Render()
{
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);
	m_context.SwapContextBuffers();
}