#include "PCH.h"
#include "RendererImpl.h"
#include "Resources\RootSignatureFactory.h"
#include "Resources\ResourceBarrier.h"
#include "Descriptors\DescriptorTable.h"


Renderer::Impl::Impl(HWND hwnd, U32 windowWidth, U32 windowHeight)
	: m_viewport(static_cast<F32>(windowWidth), static_cast<F32>(windowHeight))
	, m_scissorRect(windowWidth, windowHeight)
	, m_camera(800.0f, 600.0f)
	, m_renderQueueEnd(0)
	, m_numRenderItems(0)
	, m_numRenderItemCacheRequests(0)
{
	auto pAdapter = m_hwCaps.GetDisplayAdapters()[0].Get();
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
	m_cbDescHeap.Init(m_device.Get(), DescHeapType::CB_SR_UA, 1, true);

	// TODO: Move somewhere else
	for (size_t i = 0; i < MAX_COMMAND_LISTS; i++)
		InitCommandList(m_commandLists[i]);

	// Synchronize
	m_fence.Init(m_device.Get(), 0);
	m_currentFence = 1;
	m_handleEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	WaitForGPU();
}

void Renderer::Impl::InitCommandList(CommandList& commandList)
{
	commandList.Init(m_device.Get(), m_commandAllocator.Get());
	commandList.Close();
}

void Renderer::Impl::CreateRootSignature()
{
	RootSignatureFactory rootSigFactory(RootSignatureFactory::ALLOW_IA_LAYOUT);
	m_rootConstColorIndex = rootSigFactory.AddParameterConstants(4);
	m_rootDescViewProjIndex = rootSigFactory.AddParameterDescriptor(RootParameterType::INL_CONSTANT_BUFFER, 1);
	DescriptorTable range(DescriptorTableType::CBV, 1, 2, 0);
	m_rootDescTableIndex = rootSigFactory.AddParameterDescTable(1, &range);
	m_pRootSignature = rootSigFactory.BuildRootSignature(m_device.Get());
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

void Renderer::Impl::PopulateCommandList(CommandList& commandList, RenderItem& renderItem)
{
	// TODO: TEMP: May move somewhere else.
	commandList.Reset(m_commandAllocator.Get(), renderItem.pso.Get());

	commandList.SetViewports(&m_viewport);
	commandList.SetScissorRects(&m_scissorRect);

	commandList.SetRootSignature(m_pRootSignature.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbDescHeap.Get() };
	commandList.Get()->SetDescriptorHeaps(1, ppHeaps);

	commandList.SetRootInlineDescriptor(m_rootDescViewProjIndex, m_viewProjConstBuffer.GetGPUVirtualAddress());
	commandList.SetRootDescriptorTable(m_rootDescTableIndex, m_cbDescHeap.GetGPUHandle(0));
	commandList.SetRoot32BitConstants(m_rootConstColorIndex, 4, m_vertColor, 0);

	commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	commandList.ClearRenderTargetView(m_swapChain.GetRTVLocation(), m_clearColor, &m_scissorRect);
	commandList.SetRenderTargets(1, &m_swapChain.GetRTVLocation(), TRUE, nullptr);

	auto& mesh = m_resCache.GetMesh(renderItem.mesh);
	commandList.SetPrimitive(TRIANGLE_LIST, &mesh.GetVertBufferView(), &mesh.GetIndexBufferView());
	commandList.DrawIndexed(mesh.GetNumIndices());

	commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	commandList.Close();
	
	m_numCommandLists++;
}

void Renderer::Impl::PopulateCommandLists()
{
	m_commandAllocator.Reset();
	
	m_numCommandLists = 0;
	for (size_t i = 0; i < m_renderQueueEnd; i++)
		PopulateCommandList(m_commandLists[m_numCommandLists], m_renderItems[m_renderQueue[i]]);
}

void Renderer::Impl::ExecuteCommandLists()
{
	ID3D12CommandList* ppCommandLists[MAX_COMMAND_LISTS];
	for (size_t i = 0; i < m_numCommandLists; i++)
		ppCommandLists[i] = m_commandLists[i].Get();
	m_commandQueue.ExecuteCommandLists(ppCommandLists, m_numCommandLists);
}

void Renderer::Impl::Present()
{
	// Swap buffers
	m_swapChain.Present(m_device.Get());
}
