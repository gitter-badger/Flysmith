#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <dxgi.h>


namespace cuc
{
	struct MSAAConfig
	{
		MSAAConfig(U32 sampleCount = 1, U32 sampleQuality = 0);

		int sampleCount;
		int sampleQuality;
	};

	class SwapChainConfig
	{
	public:
		SwapChainConfig(const HWND);
		// Height and height are received from the output window at swap chain creation if they are left 0
		SwapChainConfig(const HWND, const bool bWindowed, const MSAAConfig, const U32 width = 0, const U32 height = 0);

		void SetDefault(const bool bSetCommonToDefault = true);

		void SetWidth(const U32);
		void SetHeight(const U32);
		void SetSize(const U32 width, const U32 height);
		void SetSampleCount(const U32);
		void SetSampleQuality(const U32);
		void SetWindowed(const bool);
		void SetOutputWindow(const HWND);

		const DXGI_SWAP_CHAIN_DESC& GetDescritpion() const;
		
	private:
		DXGI_SWAP_CHAIN_DESC m_description;
	};
}

#endif