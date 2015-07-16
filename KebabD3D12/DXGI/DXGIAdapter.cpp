#include "PCH.h"
#include "DXGIAdapter.h"
using namespace cuc;
using namespace Microsoft::WRL;


DXGIAdapter::DXGIAdapter(IDXGIAdapter1* pIDXGIAdapter)
	: m_pAdapter(pIDXGIAdapter)
{
	if (m_pAdapter != nullptr)
	{
		m_pAdapter->GetDesc(&m_description);
	}
}

DXGIAdapter::~DXGIAdapter()
{
}

const wchar_t* DXGIAdapter::GetName() const
{
	if (nullptr == m_pAdapter)
		return L"Software";

	return m_description.Description;
}

Microsoft::WRL::ComPtr<IDXGIAdapter1> DXGIAdapter::Get() const
{
	return m_pAdapter;
}