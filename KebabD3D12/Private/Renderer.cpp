#include "PCH.h"
#include "Renderer.h"
#include "HardwareCaps.h"

#include "StateObjects\RasterizerStateConfig.h"
#include "StateObjects\BlendStateConfig.h"

#include "Resources\ResourceConfig.h"
#include "Resources\RootSignatureFactory.h"
#include "Resources\DescriptorHeap.h"
#include "Resources\UploadHeap.h"

#include "Pipeline\PipelineStateObject.h"
#include "Pipeline\ScissorRectangle.h"
#include "Pipeline\SwapChainConfig.h"
#include "Pipeline\ShaderProgram.h"
#include "Pipeline\SwapChain.h"
#include "Pipeline\Viewport.h"

using namespace cuc;
using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "D3d12.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "d3dcompiler.lib")


// ===========================================================================================================
// Private Renderer Implementation
// ===========================================================================================================
struct Renderer::Impl
{
	// ===========================================================================================================
	// Data members 
	// ===========================================================================================================
	// Device
	Viewport m_viewport;
	ScissorRectangle m_scissorRect;
	HardwareCaps m_hwCaps;
	SwapChain m_swapChain;
	ComPtr<ID3D12Device> m_pDevice;
	
	// Pipeline
	ComPtr<ID3D12RootSignature> m_pRootSignature;
	ComPtr<ID3D12CommandQueue> m_pCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
	ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;

	// Resources
	UploadHeap m_uploadHeap;
	ComPtr<ID3D12Resource> m_pRenderTarget;
	PipelineStateObject m_pso;
	
	DescriptorHeap m_cbDescHeap;
	DescriptorHeap m_renderTargetDescHeap;

	ID3D12Resource* m_pBufVerts;
	D3D12_VERTEX_BUFFER_VIEW m_descViewBufVert;
	ID3D12Resource* m_pBufIndices;
	D3D12_INDEX_BUFFER_VIEW m_descViewBufIndices;
	
	// Synchronization
	ComPtr<ID3D12Fence> m_pFence;
	U64 m_currentFence;
	HANDLE m_handleEvent;

	// ===========================================================================================================
	// Function members 
	// ===========================================================================================================
	Impl(HWND hwnd, U32 windowWidth, U32 windowHeight);
	~Impl();
	void CreateDevice();
	void CreateCommandQueue();
	void CreateCommandAllocator();
	void LoadAssets();
	void CreateRootSignature();
	void CreatePipelineStateObject();
	void CreateDescriptorHeap();
	void CreateCommandList();
	void CreateRenderTargetView();
	void SwapBuffers();
	void WaitForGPU();
	void PopulateCommandLists();
	void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
};

// ===========================================================================================================
// Public Renderer API
// ===========================================================================================================
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
	ID3D12CommandList* ppCommandLists[] = { m_pImpl->m_pCommandList.Get() };
	m_pImpl->m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_pImpl->SwapBuffers();

	m_pImpl->WaitForGPU();
}

// ===========================================================================================================
// Renderer Impl implementation
// ===========================================================================================================
Renderer::Impl::Impl(HWND hwnd, U32 windowWidth, U32 windowHeight)
	: m_viewport(static_cast<F32>(windowWidth), static_cast<F32>(windowHeight))
	, m_scissorRect(windowWidth, windowHeight)
{
	CreateDevice();
	CreateCommandQueue();
	m_swapChain.Init(m_pCommandQueue.Get(), hwnd);
	CreateCommandAllocator();
}

Renderer::Impl::~Impl()
{
	m_pBufVerts->Release();
}

void Renderer::Impl::CreateDevice()
{
	auto pAdapter = m_hwCaps.GetDisplayAdapters()[0].Get();
	HRESULT hr = D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
	assert(SUCCEEDED(hr));

	m_hwCaps.CheckMSAASupport(m_pDevice.Get());
}

void Renderer::Impl::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	HRESULT hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));
	assert(SUCCEEDED(hr));
}

void Renderer::Impl::CreateCommandAllocator()
{
	HRESULT hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

Mesh tempMesh;
XMMATRIX model;
XMMATRIX view;
XMMATRIX projection;
XMMATRIX mvp;
#include "Camera.h"
Camera camera({ 0.0f, 0.0f, 0.0f });

void Renderer::SubmitMesh(Mesh mesh)
{	
	tempMesh = mesh;
	m_pImpl->LoadAssets();
	
	projection = camera.GetProjMatrix(0.8f, 800.0f / 600.0f);
	view = camera.GetViewMatrix();
	model = XMMatrixIdentity();
	mvp = projection * view * model;
}

void Renderer::Impl::LoadAssets()
{
	CreateRootSignature();
	CreatePipelineStateObject();
	CreateDescriptorHeap();
	CreateCommandList();
	CreateRenderTargetView();

	auto vertBufSize = tempMesh.verts.size() * sizeof(Vertex);
	auto indexBufSize = tempMesh.indices.size() * sizeof(U32);

	ResourceConfig descVBuf(ResourceType::BUFFER, vertBufSize);
	ResourceConfig descIBuf(ResourceType::BUFFER, indexBufSize);

	m_uploadHeap.Init(m_pDevice.Get(), 2);
	m_uploadHeap.Alloc(&m_pBufVerts, descVBuf.Get(), &tempMesh.verts[0], vertBufSize);
	m_uploadHeap.Alloc(&m_pBufIndices, descIBuf.Get(), &tempMesh.indices[0], indexBufSize);

	m_descViewBufVert.BufferLocation = m_pBufVerts->GetGPUVirtualAddress();
	m_descViewBufVert.StrideInBytes = sizeof(Vertex);
	m_descViewBufVert.SizeInBytes = vertBufSize;
	
	m_descViewBufIndices.BufferLocation = m_pBufIndices->GetGPUVirtualAddress();
	m_descViewBufIndices.SizeInBytes = indexBufSize;
	m_descViewBufIndices.Format = DXGI_FORMAT_R32_UINT;

	m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pFence.GetAddressOf()));
	m_currentFence = 1;

	m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_handleEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	WaitForGPU();
}

void Renderer::Impl::CreateRootSignature()
{
	RootSignatureFactory rootSigFactory(RootSignatureFactory::ALLOW_IA_LAYOUT);

	rootSigFactory.AddParameterConstants(4);

	m_pRootSignature = rootSigFactory.BuildRootSignature(m_pDevice.Get());
}

void Renderer::Impl::CreatePipelineStateObject()
{
	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	auto VS = ShaderProgram::GetCompiledShader(ShaderType::VERTEX_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestVS.hlsl");
	auto PS = ShaderProgram::GetCompiledShader(ShaderType::PIXEL_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestPS.hlsl");

	RasterizerStateConfig rastState(D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME);
	
	m_pso.Init(m_pDevice.Get(), layout, 2, m_pRootSignature.Get(), nullptr, &rastState, &VS, &PS);
}

void Renderer::Impl::CreateDescriptorHeap()
{
	m_renderTargetDescHeap.Init(m_pDevice.Get(), DescHeapType::RENDER_TARGET, 1);
	m_cbDescHeap.Init(m_pDevice.Get(), DescHeapType::CB_SR_UA, 1, true);
}

void Renderer::Impl::CreateCommandList()
{
	HRESULT hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(), m_pso.Get(), IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void Renderer::Impl::CreateRenderTargetView()
{
	// Associate the render target with the swap chain's active surface.
	m_swapChain.GetBuffer(m_pRenderTarget.GetAddressOf());
	m_pDevice->CreateRenderTargetView(m_pRenderTarget.Get(), nullptr, m_renderTargetDescHeap.GetCPUHandle(0));
}

void Renderer::Impl::SwapBuffers()
{
	m_swapChain.Present();
	CreateRenderTargetView();
}

void Renderer::Impl::WaitForGPU()
{
	auto fence = m_currentFence;
	m_pCommandQueue->Signal(m_pFence.Get(), fence);
	m_currentFence++;

	if (m_pFence->GetCompletedValue() < fence)
	{
		m_pFence->SetEventOnCompletion(fence, m_handleEvent);
		WaitForSingleObject(m_handleEvent, INFINITE);
	}
}

void Renderer::Impl::PopulateCommandLists()
{
	HRESULT hr = m_pCommandAllocator->Reset();
	assert(SUCCEEDED(hr));

	hr = m_pCommandList->Reset(m_pCommandAllocator.Get(),  // The used command allocator cannot be associated with another command list.
		m_pso.Get()); // Initial pipeline state. Not inherited from previous command list.
	assert(SUCCEEDED(hr));

	m_pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
	m_pCommandList->RSSetViewports(1, &m_viewport);
	m_pCommandList->RSSetScissorRects(1, &m_scissorRect);

	m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pCommandList->IASetVertexBuffers(0, 1, &m_descViewBufVert);
	m_pCommandList->IASetIndexBuffer(&m_descViewBufIndices);

	U32 color[] = { 0, 0, 128, 255 };
	m_pCommandList->SetGraphicsRoot32BitConstants(0, 4, color, 0);

	SetResourceBarrier(
		m_pCommandList.Get(),
		m_pRenderTarget.Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	float clearColor[] = { 0.93f, 0.5f, 0.93f, 1.0f };
	m_pCommandList->ClearRenderTargetView(m_renderTargetDescHeap.GetCPUHandle(0), clearColor, 1, &m_scissorRect);
	m_pCommandList->OMSetRenderTargets(1, &m_renderTargetDescHeap.GetCPUHandle(0), TRUE, nullptr);

	m_pCommandList->DrawIndexedInstanced(tempMesh.indices.size(), 1, 0, 0, 0);
	//m_pCommandList->DrawInstanced(tempMesh.verts.size(), 1, 0, 0);
	
	SetResourceBarrier(
		m_pCommandList.Get(),
		m_pRenderTarget.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_pCommandList->Close();
}

void Renderer::Impl::SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	D3D12_RESOURCE_BARRIER descBarrier;
	ZeroMemory(&descBarrier, sizeof(descBarrier));

	descBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	descBarrier.Transition.pResource = resource;
	descBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	descBarrier.Transition.StateBefore = stateBefore;
	descBarrier.Transition.StateAfter = stateAfter;

	commandList->ResourceBarrier(1, &descBarrier);
}