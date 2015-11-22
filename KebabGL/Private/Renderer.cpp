#include "PCH.h"
#include "Renderer.h"
#include "Dependencies\GLCore.h"
#include "RendererImpl.h"


#pragma comment(lib, "opengl32.lib")


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

void Renderer::UpdateScene(std::vector<RenderItemProxy>) const
{

}

void Renderer::UpdateView(const TransformNoScale& camTransform) const
{

}

RenderItemHandle Renderer::AddRenderItem(ResourceHandle mesh, ResourceHandle vertexShader, ResourceHandle pixelShader) const
{
	return 0;
}

ResourceHandle Renderer::CacheMesh(const VertexArray& verts, const IndexArray& indices) const
{
	return 0;
}

void Renderer::UpdateMesh(ResourceHandle, const VertexArray& verts, const IndexArray& indices) const
{

}

ResourceHandle Renderer::CacheShader(ShaderType, const std::wstring& fullPath) const
{
	return 0;
}