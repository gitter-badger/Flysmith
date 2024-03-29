#include "PCH.h"
#include "Renderer.h"
#include "RendererImpl.h"
#include "../../KebabCommon/Transform.h"

#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3dcompiler.lib")


Renderer::Renderer(const HWND hwnd, const U32 windowWidth, const U32 windowHeight)
	: m_pImpl(new Impl(hwnd, windowWidth, windowHeight))
{
}

Renderer::~Renderer()
{
	delete m_pImpl;
}

// Copy visible render components
void Renderer::UpdateScene(std::vector<RenderItemProxy> renderables) const
{
	for (size_t i = 0; i < m_pImpl->m_numRenderItemCacheRequests; i++)
	{
		auto& request = m_pImpl->m_renderItemCacheQueue[i];
		m_pImpl->m_renderItems[m_pImpl->m_numRenderItems++].Init(request.mesh, 
																 request.vertShader, 
																 request.pixelShader, 
																 m_pImpl->m_device.Get(), 
																 m_pImpl->m_cbDescHeap.GetCPUHandle(m_pImpl->m_numRenderItems - 1),
																 &m_pImpl->m_psoManager,
																 FillMode::SOLID, CullMode::NONE);
	}
	m_pImpl->m_numRenderItemCacheRequests = 0;
	
	m_pImpl->m_renderQueueEnd = 0;
	for (auto& renderable : renderables)
	{
		auto itemHandle = renderable.renderItemId;
		m_pImpl->m_renderItems[itemHandle].UpdateTransform(renderable.transform);
		m_pImpl->m_renderQueue[m_pImpl->m_renderQueueEnd++] = itemHandle;
	}
}

// Copy camera state
void Renderer::UpdateView(const TransformNoScale& transform) const
{
	m_pImpl->m_camera.Update(transform);

	auto wvp = XMMatrixTranspose(m_pImpl->m_camera.GetViewProjMatrixXM());
	XMFLOAT4X4 wvpMat;
	XMStoreFloat4x4(&wvpMat, wvp);
	memcpy(m_pImpl->m_pViewProjDataBegin, &wvpMat, sizeof(XMFLOAT4X4));
}

void Renderer::Render() const
{
	m_pImpl->PopulateCommandLists();
	m_pImpl->ExecuteCommandLists();
	m_pImpl->Present();
	m_pImpl->WaitForGPU();
}
	
ResourceHandle Renderer::CacheMesh(const VertexArray& verts, const IndexArray& indices) const
{
	return m_pImpl->m_resCache.AddMesh(verts, indices);
}

void Renderer::UpdateMesh(ResourceHandle handle, const VertexArray& verts, const IndexArray& indices) const
{
	m_pImpl->m_resCache.UpdateMesh(handle, verts, indices);
}

ResourceHandle Renderer::CacheShader(ShaderType type, const std::wstring& fullPath) const
{
	return m_pImpl->m_resCache.AddShader(type, fullPath.c_str());
}

RenderItemHandle Renderer::AddRenderItem(ResourceHandle mesh, ResourceHandle vertexShader, ResourceHandle pixelShader) const
{
	m_pImpl->m_renderItemCacheQueue[m_pImpl->m_numRenderItemCacheRequests++] = { mesh, vertexShader, pixelShader };
	return m_pImpl->m_numRenderItems + m_pImpl->m_numRenderItemCacheRequests - 1;
}