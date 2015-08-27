#pragma once
#include "PCH.h"
#include "DXGI\DXGIAdapter.h"


struct MSAAConfig
{
	MSAAConfig(U32 sampleCount = 1, U32 sampleQuality = 0);
	int sampleCount;
	int sampleQuality;
};

class HardwareCaps
{
public:
	HardwareCaps();

	// Requires existing D3D12 Device
	// Returns the highest level of MSAA available
	const MSAAConfig CheckMSAASupport(ID3D12Device*);

	const std::vector<DXGIAdapter>& GetDisplayAdapters() const;
	const MSAAConfig GetMSAASupport() const;

private:
	std::vector<DXGIAdapter> m_displayAdapters;
	MSAAConfig m_msaaSupport;

	void EnumerateDisplayAdapters();
};