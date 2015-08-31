#pragma once
#include "Renderer.h"

#include "HardwareCaps.h"
#include "RenderItem.h"
#include "Camera.h"
#include "Device.h"
#include "Fence.h"

#include "Resources\ResourceCache.h"
#include "Resources\UploadHeap.h"

#include "Pipeline\ScissorRectangle.h"
#include "Pipeline\SwapChain.h"
#include "Pipeline\Viewport.h"

#include "Pipeline\CommandAllocator.h"
#include "Pipeline\CommandQueue.h"


struct Renderer::Impl
{
	// Device
	Viewport m_viewport;
	ScissorRectangle m_scissorRect;
	HardwareCaps m_hwCaps;
	SwapChain m_swapChain;
	Device m_device;

	// Pipeline
	Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature;
	CommandQueue     m_commandQueue;
	CommandAllocator m_commandAllocator;

	static const size_t MAX_COMMAND_LISTS = 100;
	CommandList m_commandLists[MAX_COMMAND_LISTS];
	size_t m_numCommandLists;

	// Resources
	UploadHeap m_uploadHeap;

	DescriptorHeap m_cbDescHeap;

	Resource m_viewProjConstBuffer;
	U8* m_pViewProjDataBegin;
	Camera m_camera;

	// Synchronization
	Fence m_fence;
	U64 m_currentFence;
	HANDLE m_handleEvent;

	Impl(HWND hwnd, U32 windowWidth, U32 windowHeight);

	void CreateRootSignature();
	void CreateDevice();
	void WaitForGPU();

	void PopulateCommandLists();
	void PopulateCommandList(CommandList&, RenderItem& renderItem);
	void ExecuteCommandLists();
	void Present();

	ResourceCache m_resCache;
	U32 m_rootConstColorIndex;
	U32 m_rootDescViewProjIndex;
	U32 m_rootDescTableIndex;
	// TODO: Temp
	const U32 m_vertColor[4] = { 0, 0, 128, 255 };
	const F32 m_clearColor[4] = { 0.93f, 0.5f, 0.93f, 1.0f };

	static const size_t MAX_RENDER_QUEUE_ITEMS = 100;
	static const size_t MAX_RENDER_ITEMS = 100;

	// Holds handles of render items that are going to be rendered this frame.
	RenderItemHandle m_renderQueue[MAX_RENDER_QUEUE_ITEMS];
	size_t m_renderQueueEnd;

	// Holds all render items, whether they are to be rendered or not.
	RenderItem m_renderItems[MAX_RENDER_ITEMS];
	size_t m_numRenderItems;

	struct RenderItemCacheRequest
	{
		ResourceHandle mesh;
		ResourceHandle vertShader;
		ResourceHandle pixelShader;
	};

	RenderItemCacheRequest m_renderItemCacheQueue[MAX_RENDER_ITEMS];
	size_t m_numRenderItemCacheRequests;
};