#pragma once
#include "Renderer.h"

#include "HardwareCaps.h"
#include "Camera.h"
#include "Device.h"
#include "Fence.h"

#include "Resources\DescriptorHeap.h"
#include "Resources\UploadHeap.h"

#include "Pipeline\PipelineStateObject.h"
#include "Pipeline\ScissorRectangle.h"
#include "Pipeline\SwapChain.h"
#include "Pipeline\Viewport.h"

#include "Pipeline\CommandAllocator.h"
#include "Pipeline\CommandQueue.h"
#include "Pipeline\CommandList.h"

#include "Descriptors\ConstantBufferView.h"
#include "Descriptors\VertexBufferView.h"
#include "Descriptors\IndexBufferView.h"


namespace cuc
{
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
		CommandList      m_commandList;
		CommandQueue     m_commandQueue;
		CommandAllocator m_commandAllocator;

		// Resources
		UploadHeap m_uploadHeap;
		PipelineStateObject m_pso;

		DescriptorHeap m_cbDescHeap;

		ID3D12Resource* m_pVertBuffer;
		VertexBufferView m_vertBufferView;
		
		ID3D12Resource* m_pIndexBuffer;
		IndexBufferView m_indexBufferView;
		
		ID3D12Resource* m_pConstantBuffer;
		ConstantBufferView m_cbView;
		DirectX::XMFLOAT4X4 m_viewProjMat;
		Camera m_camera;

		// Synchronization
		Fence m_fence;
		U64 m_currentFence;
		HANDLE m_handleEvent;

		Impl(HWND hwnd, U32 windowWidth, U32 windowHeight);
		~Impl();
		void CreateDevice();
		void LoadAssets();
		void CreateRootSignature();
		void CreatePipelineStateObject();
		void CreateDescriptorHeap();
		void SwapBuffers();
		void WaitForGPU();
		void PopulateCommandLists();

		Mesh tempMesh;
		U32 rootConstColorIndex;
		U32 rootDescViewProjIndex;
	};
}