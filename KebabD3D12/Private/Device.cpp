#include "PCH.h"
#include "Device.h"

cuc::Device::Device()
	: m_pDevice(nullptr)
{
}

cuc::Device::~Device()
{
	if (m_pDevice)
	{
		m_pDevice->Release();
	}
}

void cuc::Device::Init(IUnknown* pAdapter, D3D_FEATURE_LEVEL featureLevel)
{
	HRESULT hr = D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pDevice));
	assert(SUCCEEDED(hr));
}

ID3D12Device* cuc::Device::Get()
{
	assert(m_pDevice != nullptr);
	return m_pDevice;
}
