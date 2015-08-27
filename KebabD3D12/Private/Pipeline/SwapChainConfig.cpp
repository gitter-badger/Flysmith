#include "PCH.h"
#include "SwapChainConfig.h"
#include "HardwareCaps.h"


SwapChainConfig::SwapChainConfig(const HWND hwnd)
{
	SetDefault();
	m_description.OutputWindow = hwnd;
}

SwapChainConfig::SwapChainConfig(const HWND hwnd, const bool bWindowed, const MSAAConfig msaa, const U32 width, const U32 height)
{
	SetDefault(false);

	m_description.BufferDesc.Width = width;
	m_description.BufferDesc.Height = height;

	m_description.SampleDesc.Count = msaa.sampleCount;
	m_description.SampleDesc.Quality = msaa.sampleQuality;

	m_description.OutputWindow = hwnd;
	m_description.Windowed = bWindowed;
}

void SwapChainConfig::SetDefault(const bool setCommonToDefault)
{
	if (setCommonToDefault)
	{
		m_description.BufferDesc.Width = 0;
		m_description.BufferDesc.Height = 0;

		m_description.SampleDesc.Count = 1;
		m_description.SampleDesc.Quality = 0;

		m_description.Windowed = TRUE;
	}

	m_description.BufferDesc.RefreshRate.Numerator = 1;
	m_description.BufferDesc.RefreshRate.Denominator = 60;
	m_description.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	m_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	m_description.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED;
	
	m_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_description.BufferCount = 2;

	m_description.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	m_description.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
}

void SwapChainConfig::SetSize(const U32 width, const U32 height)
{
	m_description.BufferDesc.Width = width;
	m_description.BufferDesc.Height = height;
}

void SwapChainConfig::SetWidth(const U32 width)
{
	m_description.BufferDesc.Width = width;
}

void SwapChainConfig::SetHeight(const U32 height)
{
	m_description.BufferDesc.Height = height;
}

void SwapChainConfig::SetSampleCount(const U32 count)
{
	m_description.SampleDesc.Count = count;
}

void SwapChainConfig::SetSampleQuality(const U32 quality)
{
	m_description.SampleDesc.Quality = quality;
}

void SwapChainConfig::SetWindowed(const bool windowed)
{
	m_description.Windowed = windowed;
}

void SwapChainConfig::SetOutputWindow(const HWND hwnd)
{
	m_description.OutputWindow = hwnd;
}

DXGI_SWAP_CHAIN_DESC& SwapChainConfig::GetDescritpion()
{
	return m_description;
}