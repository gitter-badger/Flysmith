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


namespace cuc
{
	class Window;

	class DX12Renderer
	{
	public:
		DX12Renderer(const std::shared_ptr<Window>&);
		~DX12Renderer();
		void Render();

	private:
		HardwareCaps m_hwCaps;
		std::shared_ptr<Window> m_pWindow;

		void CreateDevice();
		void CreateSwapChain();
		void CreateCommandQueue();
		void CreateCommandAllocator();

		void LoadAssets();
		void CreateRootSignature();
		void CreatePipelineStateObject();
		void CreateDescriptorHeap();
		void CreateCommandList();
		void CreateRenderTargetView(U32 bufferIndex = 0);

		void SwapBuffers();

		void WaitForGPU();
		void PopulateCommandLists();
		void SetResourceBarrier(ID3D12GraphicsCommandList* commandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

		Viewport m_viewport;
		ScissorRectangle m_scissorRect;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		U32 m_indexLastSwapBuf = 0;

		Microsoft::WRL::ComPtr<ID3D12Device>           m_pDevice;
		Microsoft::WRL::ComPtr<ID3D12Resource>         m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>     m_pCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>    m_pRootSignature;
		DescriptorHeap m_renderTargetDescHeap;
		
		Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;
		U64 m_currentFence;
		HANDLE m_handleEvent;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPSO;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pBufVerts;

		D3D12_VERTEX_BUFFER_VIEW m_descViewBufVert;
	};
}
#endif