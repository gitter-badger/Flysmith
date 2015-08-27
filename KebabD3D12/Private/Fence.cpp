#include "PCH.h"
#include "Fence.h"


Fence::Fence()
	: m_pFence(nullptr)
{
}

Fence::~Fence()
{
	if (m_pFence)
	{
		m_pFence->Release();
	}
}

void Fence::Init(ID3D12Device* pDevice, U64 initialValue, FenceFlags flags)
{
	assert(pDevice != nullptr);
	pDevice->CreateFence(initialValue, static_cast<D3D12_FENCE_FLAGS>(flags), IID_PPV_ARGS(&m_pFence));
}

ID3D12Fence * Fence::Get()
{
	assert(m_pFence != nullptr);
	return m_pFence;
}

U64 Fence::GetCompletedValue()
{
	return m_pFence->GetCompletedValue();
}

void Fence::SetEventOnCompletion(U64 value, HANDLE hEvent)
{
	HRESULT hr = m_pFence->SetEventOnCompletion(value, hEvent);
	assert(SUCCEEDED(hr));
}

void Fence::Signal(U64 value)
{
	m_pFence->Signal(value);
}
