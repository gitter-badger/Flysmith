#include "DX12Renderer.h"
#ifdef RENDERER_DX12
#include <d3dcompiler.h>
#include "Application\Window.h"
#include "DX12\SwapChainConfig.h"
#include <d3d12sdklayers.h>
#include <cassert>
using namespace cuc;
using namespace Microsoft::WRL;


DX12Renderer::DX12Renderer(const std::shared_ptr<Window>& pWindow)
	: m_pWindow(pWindow)
{
	LoadPipeline();
	LoadAssets();
}

DX12Renderer::~DX12Renderer()
{
}

HRESULT DX12Renderer::CreateDeviceAndSwapChain(const D3D_DRIVER_TYPE driverType, const DXGI_SWAP_CHAIN_DESC* pSwapChainDesc)
{
	ComPtr<ID3D12Device>       pDevice;
	ComPtr<IDXGIFactory1>      pDxgiFactory;
	ComPtr<IDXGISwapChain>     pDxgiSwapChain;
	ComPtr<ID3D12CommandQueue> pQueue;

	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&pDevice);
	assert(SUCCEEDED(hr));

	D3D12_COMMAND_QUEUE_DESC queueDesc;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY::D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.NodeMask = 0;

	hr = pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&pQueue));
	assert(SUCCEEDED(hr));

	hr = CreateDXGIFactory1(IID_PPV_ARGS(&pDxgiFactory));
	assert(SUCCEEDED(hr));

	DXGI_SWAP_CHAIN_DESC localSwapChainDesc = *pSwapChainDesc;
	hr = pDxgiFactory->CreateSwapChain(pQueue.Get(), &localSwapChainDesc, &pDxgiSwapChain);
	assert(SUCCEEDED(hr));

	hr = pDevice.Get()->QueryInterface(IID_PPV_ARGS(m_pDevice.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = pQueue.Get()->QueryInterface(IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = pDxgiSwapChain.Get()->QueryInterface(IID_PPV_ARGS(m_pSwapChain.GetAddressOf()));
	if (FAILED(hr))
	{
		reinterpret_cast<IUnknown*>(m_pDevice.GetAddressOf())->Release();
		return hr;
	}

	return S_OK;
}

void DX12Renderer::LoadPipeline()
{
	SwapChainConfig swapChainConfig(m_pWindow->GetHandle());

	HRESULT hr = CreateDeviceAndSwapChain(D3D_DRIVER_TYPE_HARDWARE, &swapChainConfig.GetDescritpion());
	if (FAILED(hr))
	{
		hr = CreateDeviceAndSwapChain(D3D_DRIVER_TYPE_WARP, &swapChainConfig.GetDescritpion());
		assert(SUCCEEDED(hr));
	}

	hr = m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(m_pCommandAllocator.GetAddressOf()));
	assert(SUCCEEDED(hr));
}

struct VERTEX { FLOAT X, Y, Z; FLOAT Color[4]; };

void DX12Renderer::LoadAssets()
{
	HRESULT hr = CreateRootSignature();
	assert(SUCCEEDED(hr));

	hr = CreatePipelineStateObject();
	assert(SUCCEEDED(hr));

	D3D12_DESCRIPTOR_HEAP_DESC descHeap;
	descHeap.NumDescriptors = 1;
	descHeap.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descHeap.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descHeap.NodeMask = 0;
	hr = m_pDevice->CreateDescriptorHeap(&descHeap, IID_PPV_ARGS(m_pDescriptorHeap.GetAddressOf()));
	assert(SUCCEEDED(hr));

	hr = m_pDevice->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAllocator.Get(),
		m_pPSO.Get(),
		IID_PPV_ARGS(m_pCommandList.GetAddressOf())
		);
	assert(SUCCEEDED(hr));

	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(m_pRenderTarget.GetAddressOf()));
	assert(SUCCEEDED(hr));
	
	m_pDevice->CreateRenderTargetView(m_pRenderTarget.Get(), nullptr, m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	assert(SUCCEEDED(hr));

	m_viewport = {
		0.0f,
		0.0f,
		static_cast<float>(m_pWindow->GetWidth()),
		static_cast<float>(m_pWindow->GetHeight()),
		0.0f, 
		1.0f
	};

	m_rectScissor.left = 0;
	m_rectScissor.top = 0;
	m_rectScissor.right = m_pWindow->GetWidth();
	m_rectScissor.bottom = m_pWindow->GetHeight();

	VERTEX triangleVerts[] =
	{
		{ 0.0f, 0.5f, 0.0f,{ 1.0f, 0.0f, 0.0f, 1.0f } },
		{ 0.45f, -0.5, 0.0f,{ 0.0f, 1.0f, 0.0f, 1.0f } },
		{ -0.45f, -0.5f, 0.0f,{ 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	D3D12_HEAP_PROPERTIES heapProperty;
	heapProperty.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 0;
	heapProperty.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC descResource;
	descResource.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
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
	const UINT64 fence = m_currentFence;
	m_pCommandQueue->Signal(m_pFence.Get(), fence);
	m_currentFence++;

	if (m_pFence->GetCompletedValue() < fence)
	{
		m_pFence->SetEventOnCompletion(fence, m_handleEvent);
		WaitForSingleObject(m_handleEvent, INFINITE);
	}
}

HRESULT DX12Renderer::CreatePipelineStateObject()
{
	ComPtr<ID3DBlob> blobShaderVert;
	ComPtr<ID3DBlob> blobErrorVert;
	HRESULT hr = D3DCompileFromFile(
		L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestVS.hlsl", 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"main", 
		"vs_5_1", 
		0,
		0, 
		blobShaderVert.GetAddressOf(), 
		blobErrorVert.GetAddressOf());
	assert(SUCCEEDED(hr));
	assert(!blobErrorVert);

	ComPtr<ID3DBlob> blobShaderPixel;
	ComPtr<ID3DBlob> blobErrorPixel;
	hr = D3DCompileFromFile(
		L"D:\\Flysmith\\KebabEngine\\Rendering\\DX12\\Shaders\\TestPS.hlsl", 
		nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", 
		"ps_5_1", 
		0, 
		0, 
		blobShaderPixel.GetAddressOf(), 
		blobErrorPixel.GetAddressOf());
	assert(SUCCEEDED(hr));
	assert(!blobErrorPixel);

	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_RASTERIZER_DESC descRasterizer;
	descRasterizer.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	descRasterizer.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	descRasterizer.FrontCounterClockwise = FALSE;
	descRasterizer.DepthBias = 0;
	descRasterizer.SlopeScaledDepthBias = 0.0f;
	descRasterizer.DepthBiasClamp = 0.0f;
	descRasterizer.DepthClipEnable = TRUE;
	descRasterizer.MultisampleEnable = FALSE;
	descRasterizer.AntialiasedLineEnable = FALSE;
	descRasterizer.ForcedSampleCount = 0;
	descRasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE::D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	D3D12_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendOp = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	descBlend.RenderTarget[0].LogicOpEnable = FALSE;
	descBlend.RenderTarget[0].LogicOp = D3D12_LOGIC_OP::D3D12_LOGIC_OP_NOOP;
	descBlend.RenderTarget[0].SrcBlend = D3D12_BLEND::D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlend = D3D12_BLEND::D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPso;
	ZeroMemory(&descPso, sizeof(descPso));
	descPso.InputLayout.pInputElementDescs = layout;
	descPso.InputLayout.NumElements = sizeof(layout) / sizeof(layout[0]);
	descPso.pRootSignature = m_pRootSignature.Get();
	descPso.VS.pShaderBytecode = reinterpret_cast<BYTE*>(blobShaderVert->GetBufferPointer());
	descPso.VS.BytecodeLength = blobShaderVert->GetBufferSize();
	descPso.PS.pShaderBytecode = reinterpret_cast<BYTE*>(blobShaderPixel->GetBufferPointer());
	descPso.PS.BytecodeLength = blobShaderPixel->GetBufferSize();
	descPso.RasterizerState = descRasterizer;
	descPso.BlendState = descBlend;
	descPso.DepthStencilState.DepthEnable = FALSE;
	descPso.DepthStencilState.StencilEnable = FALSE;
	descPso.SampleMask = UINT_MAX;
	descPso.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	descPso.NumRenderTargets = 1;
	descPso.RTVFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	descPso.SampleDesc.Count = 1;
	descPso.SampleDesc.Quality = 0;

	hr = m_pDevice->CreateGraphicsPipelineState(&descPso, IID_PPV_ARGS(m_pPSO.GetAddressOf()));
	assert(SUCCEEDED(hr));

	return S_OK;
}

HRESULT DX12Renderer::CreateRootSignature()
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

	return S_OK;
}

void DX12Renderer::Render()
{
	PopulateCommandLists();

	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_pSwapChain->Present(1, 0);
	m_indexLastSwapBuf = (1 + m_indexLastSwapBuf) % 2;
	HRESULT hr = m_pSwapChain->GetBuffer(m_indexLastSwapBuf, IID_PPV_ARGS(m_pRenderTarget.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(hr));
	m_pDevice->CreateRenderTargetView(m_pRenderTarget.Get(), nullptr, m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

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
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT, 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET);

	float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	//float clearColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pCommandList->ClearRenderTargetView(m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), clearColor, 1, &m_rectScissor);
	m_pCommandList->OMSetRenderTargets(1, &m_pDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), TRUE, nullptr);
	m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pCommandList->IASetVertexBuffers(0, 1, &m_descViewBufVert);
	m_pCommandList->DrawInstanced(3, 1, 0, 0);

	SetResourceBarrier(
		m_pCommandList.Get(), 
		m_pRenderTarget.Get(), 
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT);

	m_pCommandList->Close();
}

void DX12Renderer::SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	D3D12_RESOURCE_BARRIER descBarrier = {};
	ZeroMemory(&descBarrier, sizeof(descBarrier));

	descBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	descBarrier.Transition.pResource = resource;
	descBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	descBarrier.Transition.StateBefore = stateBefore;
	descBarrier.Transition.StateAfter = stateAfter;

	commandList->ResourceBarrier(1, &descBarrier);
}

#endif