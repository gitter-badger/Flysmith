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
void Renderer::UpdateView(const Transform& transform)
{
	m_pImpl->m_camera.Update(transform);

	//auto wvp = XMMatrixTranspose(objTransform.GetMatrixXM()) * XMMatrixTranspose(m_pImpl->m_camera.GetViewMatrixXM()) * XMMatrixTranspose(m_pImpl->m_camera.GetProjMatrixXM());
	auto wvp = objTransform.GetMatrixXM() * m_pImpl->m_camera.GetViewProjMatrixXM();
	//XMStoreFloat4x4(&m_pImpl->m_viewProjMat, XMMatrixTranspose(wvp));
	XMStoreFloat4x4(&m_pImpl->m_viewProjMat, wvp);
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
	m_pImpl->tempMesh.verts = {
		{ -1, -1, -1 }
		,{ 1, -1, -1 }
		,{ -1, 1, -1 }
		, {1, 1, -1}
		, {-1, -1, 1}
		, {1, -1, 1 }
		, {-1, 1, 1}
		, {1, 1, 1}
		/*{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f }*/
	};

	

	m_pImpl->tempMesh.indices = {
		0, 1, 2
		, 2, 1, 3
		, 7, 6, 5
		, 5, 6, 4
		, 2, 3, 6
		, 6, 3, 7
		, 3, 1, 7
		, 7, 1, 5
		, 1, 0, 5
		, 5, 0, 4
		, 0, 2, 4
		, 4, 2, 6
	};
	
	//m_pImpl->tempMesh = mesh;
	m_pImpl->LoadAssets();
}