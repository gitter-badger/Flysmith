#pragma once
#include "PCH.h"
#include <dxgi1_4.h>


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