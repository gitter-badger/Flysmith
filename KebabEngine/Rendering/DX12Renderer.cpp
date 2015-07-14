#include "DX12Renderer.h"
#ifdef RENDERER_DX12
#include "Application\Window.h"
#include "DX12\SwapChainConfig.h"
#include "DX12\ShaderProgram.h"
#include "DX12\StateObjects\RasterizerStateConfig.h"
#include "DX12\StateObjects\BlendStateConfig.h"
#include <d3d12sdklayers.h>
#include <cassert>
using namespace cuc;
using namespace Microsoft::WRL;


DX12Renderer::DX12Renderer(const std::shared_ptr<Window>& pWindow)
	: m_pWindow(pWindow)
{
	CreateDevice();
	CreateCommandQueue();
	CreateSwapChain();
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

void DX12Renderer::CreateSwapChain()
{
	IDXGIFactory4* pDXGIFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pDXGIFactory));
	assert(SUCCEEDED(hr));

	SwapChainConfig swapChainConfig(m_pWindow->GetHandle());
	hr = pDXGIFactory->CreateSwapChain(m_pCommandQueue.Get(), &swapChainConfig.GetDescritpion(), &m_pSwapChain);
	assert(SUCCEEDED(hr));

	pDXGIFactory->Release();
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
	CreateViewport();
	CreateScissorRect();

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

	RasterizerStateConfig rasterizerStateConfig;
	BlendStateConfig blendStateConfig;
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPso;
	ZeroMemory(&descPso, sizeof(descPso));
	descPso.InputLayout.pInputElementDescs = layout;
	descPso.InputLayout.NumElements = sizeof(layout) / sizeof(layout[0]);
	descPso.pRootSignature = m_pRootSignature.Get();
	descPso.VS = ShaderProgram::GetCompiledShader(ShaderType::VERTEX, L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestVS.hlsl");
	descPso.PS = ShaderProgram::GetCompiledShader(ShaderType::PIXEL, L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestPS.hlsl");
	descPso.RasterizerState = rasterizerStateConfig;
	descPso.BlendState = blendStateConfig;
	descPso.DepthStencilState.DepthEnable = FALSE;
	descPso.DepthStencilState.StencilEnable = FALSE;
	descPso.SampleMask = UINT_MAX;
	descPso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	descPso.NumRenderTargets = 1;
	descPso.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	descPso.SampleDesc.Count = 1;
	descPso.SampleDesc.Quality = 0;

	HRESULT hr = m_pDevice->CreateGraphicsPipelineState(&descPso, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DX12Renderer::CreateRootSignature()
{
	D3D12_ROOT_SIGNATURE_DESC descRootSignature;
	descRootSignature.NumParameters = 0;
	descRootSignature.pParameters = nullptr;
	descRootSignature.NumStaticSamplers = 0;
	descRootSignature.pStaticSamplers = nullptr;
	descRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ComPtr<ID3DBlob> pOutBlob, pErrorBlob;
	HRESULT hr = D3D12SerializeRootSignature(&descRootSignature, D3D_ROOT_SIGNATURE_VERSION_1, pOutBlob.GetAddressOf(), pErrorBlob.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = m_pDevice->CreateRootSignature(0, pOutBlob->GetBufferPointer(), pOutBlob->GetBufferSize(), IID_PPV_ARGS(m_pRootSignature.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DX12Renderer::CreateDescriptorHeap()
{
	m_descriptorHeap.Init(m_pDevice.Get(), DescHeapType::RENDER_TARGET);
}

void DX12Renderer::CreateCommandList()
{
	HRESULT hr = m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(),m_pPSO.Get(), IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

void DX12Renderer::CreateRenderTargetView(U32 bufferIndex)
{
	HRESULT hr = m_pSwapChain->GetBuffer(bufferIndex, IID_PPV_ARGS(m_pRenderTarget.GetAddressOf()));
	assert(SUCCEEDED(hr));

	m_pDevice->CreateRenderTargetView(m_pRenderTarget.Get(), nullptr, m_descriptorHeap.GetCPUHandle(0));
}

void DX12Renderer::CreateViewport()
{
	m_viewport = {
		0.0f,
		0.0f,
		static_cast<float>(m_pWindow->GetWidth()),
		static_cast<float>(m_pWindow->GetHeight()),
		0.0f,
		1.0f
	};
}

void cuc::DX12Renderer::CreateScissorRect()
{
	m_rectScissor.left = 0;
	m_rectScissor.top = 0;
	m_rectScissor.right = m_pWindow->GetWidth();
	m_rectScissor.bottom = m_pWindow->GetHeight();
}

void DX12Renderer::SwapBuffers()
{
	m_pSwapChain->Present(1, 0);
	m_indexLastSwapBuf = (1 + m_indexLastSwapBuf) % 2;
	CreateRenderTargetView(m_indexLastSwapBuf);
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

	hr = m_pCommandList->Reset(m_pCommandAllocator.Get(), m_pPSO.Get());
	assert(SUCCEEDED(hr));
	m_pCommandList->SetGraphicsRootSignature(m_pRootSignature.Get());
	m_pCommandList->RSSetViewports(1, &m_viewport);
	m_pCommandList->RSSetScissorRects(1, &m_rectScissor);

	SetResourceBarrier(
		m_pCommandList.Get(), 
		m_pRenderTarget.Get(), 
		D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATE_RENDER_TARGET);

	float clearColor[] = { 0.93f, 0.5f, 0.93f, 1.0f };
	m_pCommandList->ClearRenderTargetView(m_descriptorHeap.GetCPUHandle(0), clearColor, 1, &m_rectScissor);
	m_pCommandList->OMSetRenderTargets(1, &m_descriptorHeap.GetCPUHandle(0), TRUE, nullptr);
	m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pCommandList->IASetVertexBuffers(0, 1, &m_descViewBufVert);
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