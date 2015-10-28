#include "PCH.h"
#include "RendererImpl.h"
#include "Pipeline\RootSignatureFactory.h"
#include "Resources\ClearValue.h"
#include "Resources\ResourceBarrier.h"
#include "Descriptors\DescriptorTable.h"
#include "Descriptors\DepthStencilView.h"


Renderer::Impl::Impl(HWND hwnd, U32 windowWidth, U32 windowHeight)
	: m_viewport(static_cast<F32>(windowWidth), static_cast<F32>(windowHeight))
	, m_scissorRect(windowWidth, windowHeight)
	, m_camera(800.0f, 600.0f)
	, m_renderQueueEnd(0)
	, m_numRenderItems(0)
	, m_numRenderItemCacheRequests(0)
	, m_psoManager(&m_device, &m_resCache, &m_rootSignature)
{
	// TEMP: Select the second(integrated card) adapter due to some problem with nvidia I can't pinpoint.
	auto pAdapter = m_hwCaps.GetDisplayAdapter(1).Get();
	m_device.Init(pAdapter.Get());
	m_hwCaps.CheckMSAASupport(m_device.Get());

	m_commandQueue.Init(m_device.Get());
	m_swapChain.Init(m_device.Get(), m_commandQueue.Get(), hwnd);
	m_commandAllocator.Init(m_device.Get());

	CreateRootSignature();

	// Create Resources
	ResourceConfig descCB(ResourceType::BUFFER, sizeof(XMFLOAT4X4));
	m_viewProjConstBuffer.CreateCommited(m_device.Get(), descCB, &m_pViewProjDataBegin);

	// Create Descriptor Heaps
	m_cbDescHeap.Init(m_device.Get(), DescHeapType::CB_SR_UA, MAX_RENDER_ITEMS, true);
	m_dsvDescHeap.Init(m_device.Get(), DescHeapType::DEPTH_STENCIL, 1);

	// Create Depth Buffer
	ClearValue clearValue(DXGI_FORMAT_D32_FLOAT, 1.0f);
	ResourceConfig dbConfig(ResourceType::TEXTURE2D, 800, 600, DXGI_FORMAT_R32_TYPELESS, TextureLayout::UNKNOWN, ResourceFlag::ALLOW_DEPTH_STENCIL);
	m_depthBuffer.CreateCommited(m_device.Get(), dbConfig, nullptr, nullptr, 0, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue);

	DepthStencilView dsvDesc;
	m_device.Get()->CreateDepthStencilView(m_depthBuffer.Get(), &dsvDesc, m_dsvDescHeap.GetCPUHandle(0));

	// Create a command list 
	m_commandList.Init(m_device.Get(), m_commandAllocator.Get());
	m_commandList.Close();

	// Create synchronization objects
	m_fence.Init(m_device.Get(), 0);
	m_currentFence = 1;
	m_handleEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);

	// Synchronize
	WaitForGPU();

	m_resCache.Init(m_device.Get());
}

void Renderer::Impl::CreateRootSignature()
{
	RootSignatureFactory rootSigFactory(RootSignatureFactory::ALLOW_IA_LAYOUT);
	m_rootDescViewProjIndex = rootSigFactory.AddParameterDescriptor(RootParameterType::INL_CONSTANT_BUFFER, 1);
	DescriptorTable range(DescriptorTableType::CBV, 1, 2, 0);
	m_rootDescTableIndex = rootSigFactory.AddParameterDescTable(1, &range);
	rootSigFactory.BuildRootSignature(m_device.Get(), &m_rootSignature);
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
	
	m_commandList.Reset(m_commandAllocator.Get());

	m_commandList.SetViewports(&m_viewport);
	m_commandList.SetScissorRects(&m_scissorRect);

	m_commandList.SetRootSignature(m_rootSignature.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbDescHeap.Get() };
	m_commandList.Get()->SetDescriptorHeaps(1, ppHeaps);

	m_commandList.SetRootInlineDescriptor(m_rootDescViewProjIndex, m_viewProjConstBuffer.GetGPUVirtualAddress());

	m_commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	m_commandList.Get()->ClearDepthStencilView(m_dsvDescHeap.GetCPUHandle(0), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	m_commandList.ClearRenderTargetView(m_swapChain.GetRTVLocation(), m_clearColor, &m_scissorRect);
	m_commandList.SetRenderTargets(1, &m_swapChain.GetRTVLocation(), TRUE, &m_dsvDescHeap.GetCPUHandle(0));

	for (size_t i = 0; i < m_renderQueueEnd; i++)
	{
		auto& renderItem = m_renderItems[m_renderQueue[i]];

		m_commandList.SetPSO(m_psoManager.GetPSOById(renderItem.psoId).Get());

		m_commandList.SetRootDescriptorTable(m_rootDescTableIndex, m_cbDescHeap.GetGPUHandle(m_renderQueue[i])); // TODO: Change how indexing in the descriptor heap works
		
		auto& mesh = m_resCache.GetMesh(renderItem.mesh);
		m_commandList.SetPrimitive(TRIANGLE_LIST, &mesh.GetVertBufferView(), &mesh.GetIndexBufferView());
		m_commandList.DrawIndexed(mesh.GetNumIndices());
	}

	m_commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	m_commandList.Close();
}

void Renderer::Impl::ExecuteCommandLists()
{
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue.ExecuteCommandLists(ppCommandLists);
}

void Renderer::Impl::Present()
{
	// Swap buffers
	m_swapChain.Present(m_device.Get());
}
