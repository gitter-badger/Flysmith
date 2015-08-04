#include "PCH.h"
#include "RendererImpl.h"

#include "Resources\RootSignatureFactory.h"
#include "Resources\ResourceBarrier.h"
#include "Resources\ResourceConfig.h"

using namespace cuc;
using namespace DirectX;


Renderer::Impl::Impl(HWND hwnd, U32 windowWidth, U32 windowHeight)
	: m_viewport(static_cast<F32>(windowWidth), static_cast<F32>(windowHeight))
	, m_scissorRect(windowWidth, windowHeight)
	, m_camera(800.0f / 600.0f)
{
	auto pAdapter = m_hwCaps.GetDisplayAdapters()[0].Get();
	m_device.Init(pAdapter.Get());
	m_hwCaps.CheckMSAASupport(m_device.Get());

	m_commandQueue.Init(m_device.Get());
	m_swapChain.Init(m_device.Get(), m_commandQueue.Get(), hwnd);
	m_commandAllocator.Init(m_device.Get());
}

Renderer::Impl::~Impl()
{
	if (m_pVertBuffer) m_pVertBuffer->Release();
	if (m_pIndexBuffer) m_pIndexBuffer->Release();
}

void Renderer::Impl::LoadAssets()
{
	// 1. Create RootSignature
	CreateRootSignature();

	// 2. Create PSO
	CreatePipelineStateObject();

	// 3. Create Resource Heaps
	m_uploadHeap.Init(m_device.Get(), 3);

	// 4. Create Resources
	auto vertBufSize = tempMesh.verts.size() * sizeof(Vertex);
	ResourceConfig descVBuf(ResourceType::BUFFER, vertBufSize);
	m_uploadHeap.Alloc(&m_pVertBuffer, descVBuf.Get(), &tempMesh.verts[0], vertBufSize);
	
	auto indexBufSize = tempMesh.indices.size() * sizeof(U32);
	ResourceConfig descIBuf(ResourceType::BUFFER, indexBufSize);
	m_uploadHeap.Alloc(&m_pIndexBuffer, descIBuf.Get(), &tempMesh.indices[0], indexBufSize);

	D3D12_HEAP_PROPERTIES heapProperties;
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 0;
	heapProperties.VisibleNodeMask = 0;

	ResourceConfig descCB(ResourceType::BUFFER, sizeof(m_viewProjMat));

	m_device.Get()->CreateCommittedResource(&heapProperties, 
											D3D12_HEAP_FLAG_NONE, 
											&descCB.Get(), 
											D3D12_RESOURCE_STATE_GENERIC_READ, 
											nullptr,
											IID_PPV_ARGS(&m_pConstantBuffer));

	m_pConstantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pCBDataBegin));

	// 5. Create Descriptor Heaps
	CreateDescriptorHeap();
	
	// 6. Create Command List
	m_commandList.Init(m_device.Get(), m_commandAllocator.Get(), m_pso.Get());
	
	// 7. Create Resource Descriptors
	m_vertBufferView.Init(m_pVertBuffer->GetGPUVirtualAddress(), vertBufSize, sizeof(Vertex));
	m_indexBufferView.Init(m_pIndexBuffer->GetGPUVirtualAddress(), indexBufSize);
	
	// 8. Synchronize
	m_fence.Init(m_device.Get(), 0);
	m_currentFence = 1;

	m_commandList.Close();

	m_handleEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	WaitForGPU();
}

void Renderer::Impl::CreateRootSignature()
{
	RootSignatureFactory rootSigFactory(RootSignatureFactory::ALLOW_IA_LAYOUT);
	rootConstColorIndex = rootSigFactory.AddParameterConstants(4);
	rootDescViewProjIndex = rootSigFactory.AddParameterDescriptor(RootParameterType::INL_CONSTANT_BUFFER, 1);
	m_pRootSignature = rootSigFactory.BuildRootSignature(m_device.Get());
}

void Renderer::Impl::CreatePipelineStateObject()
{
	D3D12_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	auto VS = ShaderProgram::GetCompiledShader(ShaderType::VERTEX_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestVS.hlsl");
	auto PS = ShaderProgram::GetCompiledShader(ShaderType::PIXEL_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestPS.hlsl");

	RasterizerStateConfig rastState(D3D12_FILL_MODE_WIREFRAME, D3D12_CULL_MODE_NONE);

	m_pso.Init(m_device.Get(), layout, 2, m_pRootSignature.Get(), nullptr, &rastState, &VS, &PS);
}

void Renderer::Impl::CreateDescriptorHeap()
{
	m_cbDescHeap.Init(m_device.Get(), DescHeapType::CB_SR_UA, 1, true);
}

void Renderer::Impl::WaitForGPU()
{
	auto fence = m_currentFence;
	m_commandQueue.Signal(m_fence.Get(), fence);
	m_currentFence++;

	if (m_fence.GetCompletedValue() < fence)
	{
		m_fence.SetEventOnCompletion(fence, m_handleEvent);
		WaitForSingleObject(m_handleEvent, INFINITE);
	}
}

void Renderer::Impl::PopulateCommandLists()
{
	m_commandAllocator.Reset();

	m_commandList.Reset(m_commandAllocator.Get(), m_pso.Get());

	m_commandList.SetViewports(&m_viewport);
	m_commandList.SetScissorRects(&m_scissorRect);

	m_commandList.SetPrimitive(TRIANGLE_LIST, &m_vertBufferView, &m_indexBufferView);

	m_commandList.SetRootSignature(m_pRootSignature.Get());

	// Set root signature inline constants
	U32 color[] = { 0, 0, 128, 255 };
	m_commandList.SetRoot32BitConstants(rootConstColorIndex, 4, color, 0);

	// Set root signature inline descriptors
	m_commandList.Get()->SetGraphicsRootConstantBufferView(rootDescViewProjIndex, m_pConstantBuffer->GetGPUVirtualAddress());

	m_commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	float clearColor[] = { 0.93f, 0.5f, 0.93f, 1.0f };
	m_commandList.ClearRenderTargetView(m_swapChain.GetRTVLocation(), clearColor, &m_scissorRect);
	m_commandList.SetRenderTargets(1, &m_swapChain.GetRTVLocation(), TRUE, nullptr);

	m_commandList.DrawIndexed(tempMesh.indices.size());

	m_commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_commandList.Close();
}
