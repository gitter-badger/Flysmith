#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>
#include <wrl/client.h>
#include <memory>
#include "DX12\HardwareCaps.h"
#include "DX12\DescriptorHeap.h"
#include "DX12\ScissorRectangle.h"
#include "DX12\Viewport.h"
#include "DX12\SwapChain.h"
#include "DX12\PipelineStateObject.h"
#include "Events\EventListener.h"
#include "DX12\UploadHeap.h"


namespace cuc
{
	class Window;

	class DX12Renderer : public EventListener
	{
	public:
		DX12Renderer(const std::shared_ptr<Window>&);
		~DX12Renderer();
		void Render();
		void HandleEvent(const Event&);

	private:
		HardwareCaps m_hwCaps;
		std::shared_ptr<Window> m_pWindow;

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
#endif