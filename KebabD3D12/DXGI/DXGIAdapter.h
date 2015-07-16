#pragma once
#include "PCH.h"


namespace cuc
{
	class DXGIAdapter
	{
		friend class DX12Renderer;

	public:
		DXGIAdapter(IDXGIAdapter1*);
		~DXGIAdapter();
		const wchar_t* GetDescription() const;

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter1> m_pAdapter;
	};
}