#include "PCH.h"
#include "RendererImpl.h"

#include "Resources\RootSignatureFactory.h"
#include "Resources\ResourceBarrier.h"
#include "Resources\ResourceConfig.h"

#include "Descriptors\DescriptorTable.h"
#include "Descriptors\ConstantBufferView.h"

using namespace DirectX;


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
}

Renderer::Impl::~Impl()
{
}

void Renderer::Impl::LoadAssets()
{
	// Create RootSignature
	CreateRootSignature();

	// Create Resources
	ResourceConfig descCB(ResourceType::BUFFER, sizeof(XMFLOAT4X4));
	m_viewProjConstBuffer.CreateCommited(m_device.Get(), descCB, &m_pViewProjDataBegin);

	// Create Descriptor Heaps
	m_cbDescHeap.Init(m_device.Get(), DescHeapType::CB_SR_UA, 1, true);

	// Create Command List
	m_commandList.Init(m_device.Get(), m_commandAllocator.Get());

	// Synchronize
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
	DescriptorTable range(DescriptorTableType::CBV, 1, 2, 0);
	rootSigFactory.AddParameterDescTable(1, &range);
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

void Renderer::Impl::PopulateCommandLists()
{
	m_commandAllocator.Reset();
	m_commandList.Reset(m_commandAllocator.Get(), m_renderItems[0].pso.Get());

	m_commandList.SetViewports(&m_viewport);
	m_commandList.SetScissorRects(&m_scissorRect);

	m_commandList.SetRootSignature(m_pRootSignature.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbDescHeap.Get() };
	m_commandList.Get()->SetDescriptorHeaps(1, ppHeaps);

	m_commandList.Get()->SetGraphicsRootDescriptorTable(2, m_cbDescHeap.GetGPUHandle(0));

	// Set root signature inline constants
	U32 color[] = { 0, 0, 128, 255 };
	m_commandList.SetRoot32BitConstants(rootConstColorIndex, 4, color, 0);

	// Set root signature inline descriptors
	m_commandList.Get()->SetGraphicsRootConstantBufferView(rootDescViewProjIndex, m_viewProjConstBuffer.GetGPUVirtualAddress());

	m_commandList.SetResourceBarriers(&TransitionBarrier(m_swapChain.GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	float clearColor[] = { 0.93f, 0.5f, 0.93f, 1.0f };
	m_commandList.ClearRenderTargetView(m_swapChain.GetRTVLocation(), clearColor, &m_scissorRect);
	m_commandList.SetRenderTargets(1, &m_swapChain.GetRTVLocation(), TRUE, nullptr);

	m_commandList.SetPrimitive(TRIANGLE_LIST, &m_resCache.GetMesh(0).GetVertBufferView(), &m_resCache.GetMesh(0).GetIndexBufferView());
	m_commandList.DrawIndexed(m_resCache.GetMesh(0).GetNumIndices());

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
