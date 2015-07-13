#include "DXGIAdapter.h"
#ifdef RENDERER_DX12
using namespace cuc;
using namespace Microsoft::WRL;


DXGIAdapter::DXGIAdapter(IDXGIAdapter1* pIDXGIAdapter)
	: m_pAdapter(pIDXGIAdapter)
{
}

DXGIAdapter::~DXGIAdapter()
{
}

const wchar_t* DXGIAdapter::GetDescription() const
{
	if (nullptr == m_pAdapter)
		return L"Software";

	DXGI_ADAPTER_DESC description;
	m_pAdapter->GetDesc(&description);
	return description.Description;
}

#endif