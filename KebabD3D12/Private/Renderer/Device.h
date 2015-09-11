#pragma once
#include "PCH.h"


class Device
{
public:
	Device();
	~Device();
	void Init(IUnknown* pAdapter, D3D_FEATURE_LEVEL = D3D_FEATURE_LEVEL_11_0);
	ID3D12Device* Get();

private:
	ID3D12Device* m_pDevice;
};