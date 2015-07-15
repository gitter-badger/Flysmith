#include "DX12Renderer.h"
#ifdef RENDERER_DX12
#include "Application\Window.h"
#include "DX12\SwapChainConfig.h"
#include "DX12\ShaderProgram.h"
#include "DX12\StateObjects\RasterizerStateConfig.h"
#include "DX12\StateObjects\BlendStateConfig.h"
#include "DX12\RootSignatureFactory.h"
#include <d3d12sdklayers.h>
#include "Events\WindowEvents.h"
#include <cassert>
using namespace cuc;
using namespace Microsoft::WRL;


DX12Renderer::DX12Renderer(const std::shared_ptr<Window>& pWindow)
	: m_pWindow(pWindow)
	, m_scissorRect(pWindow.get())
	, m_viewport(pWindow.get())
{
	RegisterForEvent("WindowResize"_HASH);

	CreateDevice();
	CreateCommandQueue();
	m_swapChain.Init(m_pCommandQueue.Get(), m_pWindow->GetHandle());
	CreateCommandAllocator();

	LoadAssets();
}

DX12Renderer::~DX12Renderer()
{
}

void DX12Renderer::CreateDevice()
{
	auto pAdapter = m_hwCaps.GetDisplayAdapters()[0].m_pAdapter;
	HRESULT hr = D3D12CreateDevice(pAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
	assert(SUCCEEDED(hr));

	m_hwCaps.CheckMSAASupport(m_pDevice.Get());
}

void DX12Renderer::CreateCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	HRESULT hr = m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));
	assert(SUCCEEDED(hr));
}

void DX12Renderer::CreateCommandAllocator()
{
	HRESULT hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

struct VERTEX { FLOAT X, Y, Z; FLOAT Color[4]; };

void DX12Renderer::LoadAssets()
{
	CreateRootSignature();
	CreatePipelineStateObject();
	CreateDescriptorHeap();
	CreateCommandList();
	CreateRenderTargetView();

	VERTEX triangleVerts[] =
	{
		{ 0.0f, 0.5f, 0.0f,{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ 0.45f, -0.5, 0.0f,{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ -0.45f, -0.5f, 0.0f,{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	D3D12_HEAP_PROPERTIES heapProperty;
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 0;
	heapProperty.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC descResource;
	descResource.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	descResource.Alignment = 0;
	descResource.Width = 3 * sizeof(VERTEX);
	descResource.Height = 1;
	descResource.DepthOrArraySize = 1;
	descResource.MipLevels = 1;
	descResource.Format = DXGI_FORMAT_UNKNOWN;
	descResource.SampleDesc.Count = 1;
	descResource.SampleDesc.Quality = 0;
	descResource.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	descResource.Flags = D3D12_RESOURCE_FLAG_NONE;

	m_pDevice->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&descResource,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,  
		IID_PPV_ARGS(m_pBufVerts.GetAddressOf()));

	UINT8* dataBegin;
	m_pBufVerts->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin));
	memcpy(dataBegin, triangleVerts, sizeof(triangleVerts));
	m_pBufVerts->Unmap(0, nullptr);

	m_descViewBufVert.BufferLocation = m_pBufVerts->GetGPUVirtualAddress();
	m_descViewBufVert.StrideInBytes = sizeof(VERTEX);
	m_descViewBufVert.SizeInBytes = sizeof(triangleVerts);

	m_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_pFence.GetAddressOf()));
	m_currentFence = 1;

	m_pCommandList->Close();
	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_handleEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	WaitForGPU();
}

void DX12Renderer::CreateRootSignature()
{
	RootSignatureFactory rootSigFactory(RootSignatureFactory::ALLOW_IA_LAYOUT);
	
	rootSigFactory.AddParameterConstants(4);
	
	m_pRootSignature = rootSigFactory.BuildRootSignature(m_pDevice.Get());
}

void DX12Renderer::WaitForGPU()
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

void DX12Renderer::CreatePipelineStateObject()
{
	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	auto VS = ShaderProgram::GetCompiledShader(ShaderType::VERTEX, L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestVS.hlsl");
	auto PS = ShaderProgram::GetCompiledShader(ShaderType::PIXEL, L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestPS.hlsl");
	
	m_pso.Init(m_pDevice.Get(), layout, 2, m_pRootSignature.Get(), nullptr, nullptr, &VS, &PS);
}

void DX12Renderer::CreateDescriptorHeap()
{
	m_renderTargetDescHeap.Init(m_pDevice.Get(), DescHeapType::RENDER_TARGET, 1);
}

void DX12Renderer::CreateCommandList()
{
	HRESULT hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(), m_pso.Get(), IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DX12Renderer::CreateRenderTargetView()
{
	// Associate the render target with the swap chain's active surface.
	m_swapChain.GetBuffer(m_pRenderTarget.GetAddressOf());
	m_pDevice->CreateRenderTargetView(m_pRenderTarget.Get(), nullptr, m_renderTargetDescHeap.GetCPUHandle(0));
}

void DX12Renderer::HandleEvent(const Event& event)
{
	switch (event.type)
	{
	case "WindowResize"_HASH:
	{
		auto newWidth = event[WindowResizeEvent::U32_WIDTH].GetUnsignedInt();
		auto newHeight = event[WindowResizeEvent::U32_HEIGHT].GetUnsignedInt();
		m_swapChain.Resize(newWidth, newHeight);
		m_viewport.Resize(newWidth, newHeight);
		m_scissorRect.Resize(newWidth, newHeight);
		break;
	}
	}
}

void DX12Renderer::SwapBuffers()
{
	m_swapChain.Present();
	CreateRenderTargetView();
}

void DX12Renderer::Render()
{
	PopulateCommandLists();

	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	SwapBuffers();

	WaitForGPU();
}

void DX12Renderer::PopulateCommandLists()
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
	m_pCommandList->DrawInstanced(3, 1, 0, 0);

	SetResourceBarrier(
		m_pCommandList.Get(), 
		m_pRenderTarget.Get(), 
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	m_pCommandList->Close();
}

void DX12Renderer::SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
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

#endif