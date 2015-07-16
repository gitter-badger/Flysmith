#pragma once
#include "PCH.h"
#include <dxgi1_4.h>


namespace cuc
{
	struct MSAAConfig;

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

		DXGI_SWAP_CHAIN_DESC& GetDescritpion();
		
	private:
		DXGI_SWAP_CHAIN_DESC m_description;
	};
}