#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>
#include <dxgi1_2.h>

namespace cuc
{
	class SwapChain
	{
	public:
		SwapChain();
		~SwapChain();
		void Init(ID3D12CommandQueue*, const HWND);
		void Present(const bool bVsync = true);
		void Resize(const U32 width, const U32 height);
		void GetBuffer(ID3D12Resource** ppResourceOut);

	private:
		IDXGISwapChain1* m_pSwapChain;
		bool m_bInitialized;
		U32 m_indexLastBufSwap;
	};
}

#endif