#include "PCH.h"
#include "Device.h"


Device::Device()
	: m_pDevice(nullptr)
{
}

Device::~Device()
{
	if (m_pDevice)
	{
		m_pDevice->Release();
	}
}

void Device::Init(IUnknown* pAdapter, D3D_FEATURE_LEVEL featureLevel)
{
	HRESULT hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
	assert(SUCCEEDED(hr));
}

ID3D12Device* Device::Get()
{
	assert(m_pDevice != nullptr);
	return m_pDevice;
}
