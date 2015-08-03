#include "PCH.h"
#include "Renderer.h"
#include "RendererImpl.h"

using namespace cuc;
using namespace DirectX;

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
#include <cmath>
XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
void Renderer::Update()
{
	float yOffset = ((rand() % 200) / 100.0f - 1.0f) / 100.0f;
	pos.x++;
	m_pImpl->m_camera.SetPosition(pos);
	//memcpy(m_pImpl->m_pCBDataBegin, &m_pImpl->m_viewProjMat, sizeof(m_pImpl->m_viewProjMat));
	m_pImpl->m_offset.x -= 0.001f;
	m_pImpl->m_offset.y += yOffset;
	memcpy(m_pImpl->m_pCBDataBegin, &m_pImpl->m_offset, sizeof(m_pImpl->m_offset));
}

void Renderer::Render()
{
	// 1. Update per-frame data
	XMStoreFloat4x4(&m_pImpl->m_viewProjMat, m_pImpl->m_camera.GetProjMatrix(0.8f, 800.0f / 600.0f) *
											 m_pImpl->m_camera.GetViewMatrix());
	
	// 2. Populate command lists
	m_pImpl->PopulateCommandLists();

	// 3. Execute command lists
	ID3D12CommandList* ppCommandLists[] = { m_pImpl->m_commandList.Get() };
	m_pImpl->m_commandQueue.ExecuteCommandLists(ppCommandLists);

	// 4. Swap buffers
	m_pImpl->m_swapChain.Present(m_pImpl->m_device.Get());

	// 5. Synchronize
	m_pImpl->WaitForGPU();
}
	
void Renderer::SubmitMesh(Mesh mesh)
{	
	m_pImpl->tempMesh = mesh;
	m_pImpl->LoadAssets();
}