#pragma once
#include "PCH.h"
#include "HardwareCaps.h"
#include "DescriptorHeap.h"
#include "ScissorRectangle.h"
#include "Viewport.h"
#include "SwapChain.h"
#include "PipelineStateObject.h"
#include "UploadHeap.h"


namespace cuc
{
	class KEBAB12_API DX12Renderer
	{
	public:
		DX12Renderer(const HWND, const U32 windowWidth, const U32 windowHeight);
		~DX12Renderer();
		void Render();

	private:
		HardwareCaps m_hwCaps;

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

		Viewport m_viewport;
		ScissorRectangle m_scissorRect;
		SwapChain m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12Device>           m_pDevice;
		Microsoft::WRL::ComPtr<ID3D12Resource>         m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>     m_pCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>    m_pRootSignature; 
		DescriptorHeap m_renderTargetDescHeap;
		UploadHeap m_uploadHeap;
		ID3D12Resource* m_pBufVerts;
		
		Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;
		U64 m_currentFence;
		HANDLE m_handleEvent;

		PipelineStateObject m_pso;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList;

		D3D12_VERTEX_BUFFER_VIEW m_descViewBufVert;
	};
}