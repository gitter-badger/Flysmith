#include "PCH.h"
#include "Fence.h"

cuc::Fence::Fence()
	: m_pFence(nullptr)
{
}

cuc::Fence::~Fence()
{
	if (m_pFence)
	{
		m_pFence->Release();
	}
}

void cuc::Fence::Init(ID3D12Device* pDevice, U64 initialValue, FenceFlags flags)
{
	assert(pDevice != nullptr);
	pDevice->CreateFence(initialValue, static_cast<D3D12_FENCE_FLAGS>(flags), IID_PPV_ARGS(&m_pFence));
}

ID3D12Fence * cuc::Fence::Get()
{
	assert(m_pFence != nullptr);
	return m_pFence;
}

U64 cuc::Fence::GetCompletedValue()
{
	return m_pFence->GetCompletedValue();
}

void cuc::Fence::SetEventOnCompletion(U64 value, HANDLE hEvent)
{
	HRESULT hr = m_pFence->SetEventOnCompletion(value, hEvent);
	assert(SUCCEEDED(hr));
}

void cuc::Fence::Signal(U64 value)
{
	m_pFence->Signal(value);
}
