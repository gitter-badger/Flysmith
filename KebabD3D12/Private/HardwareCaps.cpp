#include "PCH.h"
#include "HardwareCaps.h"
#include <dxgi1_4.h>


MSAAConfig::MSAAConfig(U32 sampleCount, U32 sampleQuality)
	: sampleCount(sampleCount)
	, sampleQuality(sampleQuality)
{
	assert(sampleQuality == (sampleCount - 1));
}

HardwareCaps::HardwareCaps()
{
	EnumerateDisplayAdapters();
}

const MSAAConfig HardwareCaps::CheckMSAASupport(ID3D12Device* pDevice)
{
	assert(pDevice != nullptr);
	MSAAConfig tempConfig;

	HRESULT hr = S_OK;
	while(hr == S_OK)
	{
		m_msaaSupport = tempConfig;

		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS support;
		support.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		
		tempConfig.sampleCount = tempConfig.sampleCount << 1;
		support.SampleCount = tempConfig.sampleCount;
		
		// Isn't the CheckFeatureSupport func supposed to give me the MSAA quality level?
		// https://msdn.microsoft.com/en-us/library/ff476499.aspx
		// "the definition of a quality level is up to each hardware vendor to define"
		tempConfig.sampleQuality = tempConfig.sampleCount - 1;
		support.NumQualityLevels = tempConfig.sampleQuality;

		hr = pDevice->CheckFeatureSupport(
			D3D12_FEATURE::D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&support,
			sizeof(support));
	}

	return m_msaaSupport;
}

const std::vector<DXGIAdapter>& HardwareCaps::GetDisplayAdapters() const
{
	return m_displayAdapters;
}

const MSAAConfig HardwareCaps::GetMSAASupport() const
{
	return m_msaaSupport;
}

void HardwareCaps::EnumerateDisplayAdapters()
{
	IDXGIFactory4* pFactory = nullptr;
	HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
	assert(SUCCEEDED(hr));

#pragma warning(push)
#pragma warning(disable:4267)
	bool bFoundDedicated = false;
	IDXGIAdapter1* pAdapter;
	while (pFactory->EnumAdapters1(m_displayAdapters.size(), &pAdapter) != DXGI_ERROR_NOT_FOUND)
	{
		m_displayAdapters.push_back(pAdapter);
		
		// Try placing the dedicated card adapter at the start of the container.
		U32 currentIndex = m_displayAdapters.size() - 1;
		if (currentIndex != 0 && !bFoundDedicated)
		{
			auto label = m_displayAdapters[m_displayAdapters.size() - 1].GetName();	
			if (wcsstr(label, L"NVIDIA") || wcsstr(label, L"ATI"))
			{
				bFoundDedicated = true;
				std::iter_swap(m_displayAdapters.begin(), m_displayAdapters.begin() + currentIndex);
			}
		}
	}
#pragma warning(pop)

	pFactory->Release();
}