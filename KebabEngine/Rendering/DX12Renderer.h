#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>
#include <wrl/client.h>
#include <memory>


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
		std::shared_ptr<Window> m_pWindow;

		HRESULT CreateDeviceAndSwapChain(const D3D_DRIVER_TYPE, const DXGI_SWAP_CHAIN_DESC*);

		void LoadPipeline();
		void LoadAssets();

		D3D12_VIEWPORT m_viewport;
		D3D12_RECT m_rectScissor;

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		U32 m_indexLastSwapBuf = 0;

		Microsoft::WRL::ComPtr<ID3D12Device>         m_pDevice;
		Microsoft::WRL::ComPtr<ID3D12Resource>       m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID3D12Device>         m_pCommandAllocator;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue>   m_pCommandQueue;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>  m_pRootSiganture;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDescriptorHeap;
		
		Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence;
		UINT64 m_currentFence;
		HANDLE m_handleEvent;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPSO;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pCommandList;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pBufVerts;

		D3D12_VERTEX_BUFFER_VIEW m_descViewBufVert;
	};
}
#endif