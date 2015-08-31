#include "PCH.h"
#include "Renderer.h"
#include "RendererImpl.h"
#include "Transform.h"
#include "RenderComponent.h"

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
void Renderer::UpdateScene(const std::vector<RenderComponent>& renderables)
{
	for (auto& cacheRequest : m_pImpl->m_renderItemCacheQueue)
	{
		m_pImpl->m_renderItems.push_back(cacheRequest);
	}
	m_pImpl->m_renderItemCacheQueue.erase(m_pImpl->m_renderItemCacheQueue.begin(), m_pImpl->m_renderItemCacheQueue.end());

	m_pImpl->m_renderQueueEnd = 0;
	for (auto& renderable : renderables)
	{
		auto itemHandle = renderable.GetRenderItem();
		m_pImpl->m_renderItems[itemHandle].transform = renderable.m_transform;
		m_pImpl->m_renderQueue[m_pImpl->m_renderQueueEnd++] = itemHandle;
	}

	XMFLOAT4X4 worldMat;
	XMStoreFloat4x4(&worldMat, XMMatrixTranspose(m_pImpl->m_renderItems[0].transform.GetMatrixXM()));
	memcpy(m_pImpl->m_pWorldMatDataBegin, &worldMat, sizeof(XMFLOAT4X4));
}

// Copy camera state
void Renderer::UpdateView(const TransformNoScale& transform)
{
	m_pImpl->m_camera.Update(transform);

	auto wvp = XMMatrixTranspose(m_pImpl->m_camera.GetViewProjMatrixXM());
	XMFLOAT4X4 wvpMat;
	XMStoreFloat4x4(&wvpMat, wvp);
	memcpy(m_pImpl->m_pViewProjDataBegin, &wvpMat, sizeof(XMFLOAT4X4));
}

void Renderer::Render()
{
	m_pImpl->PopulateCommandLists();
	m_pImpl->ExecuteCommandLists();
	m_pImpl->Present();
	m_pImpl->WaitForGPU();
}
	
ResourceHandle Renderer::CacheMesh(const std::vector<Vertex>& verts, const std::vector<U32>& indices)
{
	m_pImpl->LoadAssets();
	auto newMeshHandle = m_pImpl->m_resCache.AddMesh(m_pImpl->m_device.Get(), verts, indices);
	return newMeshHandle;
}

ResourceHandle Renderer::CacheShader(ShaderType type, const std::wstring& fullPath)
{
	auto handle = m_pImpl->m_resCache.AddShader(type, fullPath.c_str());
	return handle;
}

RenderItemHandle Renderer::AddRenderItem(ResourceHandle mesh, ResourceHandle vertexShader, ResourceHandle pixelShader)
{
	m_pImpl->m_renderItemCacheQueue.push_back(RenderItem(mesh, vertexShader, pixelShader));
	return m_pImpl->m_renderItems.size() - 1 + m_pImpl->m_renderItemCacheQueue.size();
}