#pragma once
#include "PCH.h"


namespace cuc
{
	class DXGIAdapter
	{
	public:
		DXGIAdapter(IDXGIAdapter1*);
		~DXGIAdapter();
		const wchar_t* GetDescription() const;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> Get() const;

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter1> m_pAdapter;
	};
}