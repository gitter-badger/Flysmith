#include "PCH.h"
#include "Renderer.h"
#include "RendererImpl.h"
#include "Transform.h"

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

Transform objTransform;

// Copy visible render components
void Renderer::UpdateScene(const Transform& tempSingleEntity)
{
	objTransform = tempSingleEntity;

	m_pImpl->m_viewProjMat = objTransform.GetMatrix();
	memcpy(m_pImpl->m_pCBDataBegin, &m_pImpl->m_viewProjMat, sizeof(m_pImpl->m_viewProjMat));
}

// Copy camera state
void Renderer::UpdateView(const Transform& camTransform)
{
	XMStoreFloat4x4(&m_pImpl->m_viewProjMat, m_pImpl->m_camera.GetViewProjMatrixXM() * objTransform.GetMatrixXM());
	memcpy(m_pImpl->m_pCBDataBegin, &m_pImpl->m_viewProjMat, sizeof(m_pImpl->m_viewProjMat));
}

void Renderer::Render()
{
	// Populate command lists
	m_pImpl->PopulateCommandLists();

	// Execute command lists
	ID3D12CommandList* ppCommandLists[] = { m_pImpl->m_commandList.Get() };
	m_pImpl->m_commandQueue.ExecuteCommandLists(ppCommandLists);

	// Swap buffers
	m_pImpl->m_swapChain.Present(m_pImpl->m_device.Get());

	// Synchronize
	m_pImpl->WaitForGPU();
}
	
void Renderer::SubmitMesh(Mesh mesh)
{	
	m_pImpl->tempMesh = mesh;
	m_pImpl->LoadAssets();
}