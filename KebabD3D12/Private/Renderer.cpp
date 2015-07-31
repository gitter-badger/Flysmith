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

void Renderer::Render()
{
	m_pImpl->PopulateCommandLists();
	ID3D12CommandList* ppCommandLists[] = { m_pImpl->m_commandList.Get() };
	m_pImpl->m_commandQueue.ExecuteCommandLists(ppCommandLists);

	m_pImpl->SwapBuffers();

	m_pImpl->WaitForGPU();
}

XMMATRIX viewProjMat;
#include "Camera.h"

void Renderer::SubmitMesh(Mesh mesh)
{	
	m_pImpl->tempMesh = mesh;
	m_pImpl->LoadAssets();
	
	Camera camera({ 0.0f, 0.0f, 0.0f });
	XMMATRIX projection = camera.GetProjMatrix(0.8f, 800.0f / 600.0f);
	XMMATRIX view = camera.GetViewMatrix();
	viewProjMat = projection * view;
}