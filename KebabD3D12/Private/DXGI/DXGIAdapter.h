#pragma once
#include "PCH.h"


class DXGIAdapter
{
public:
	DXGIAdapter(IDXGIAdapter1*);
	~DXGIAdapter();
	const wchar_t* GetName() const;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> Get() const;

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter1> m_pAdapter;
	DXGI_ADAPTER_DESC m_description;
};