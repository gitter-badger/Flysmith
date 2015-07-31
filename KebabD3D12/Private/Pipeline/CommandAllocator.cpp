#include "PCH.h"
#include "CommandAllocator.h"


cuc::CommandAllocator::CommandAllocator()
	: m_pCommandAllocator(nullptr)
{
}

cuc::CommandAllocator::~CommandAllocator()
{
	if (m_pCommandAllocator)
	{
		m_pCommandAllocator->Release();
	}
}

void cuc::CommandAllocator::Init(ID3D12Device* pDevice, CommandListType type)
{
	assert(pDevice != nullptr);
	assert(m_pCommandAllocator == nullptr);

	HRESULT hr = pDevice->CreateCommandAllocator(static_cast<D3D12_COMMAND_LIST_TYPE>(type), IID_PPV_ARGS(&m_pCommandAllocator));
	assert(SUCCEEDED(hr));
}

ID3D12CommandAllocator* cuc::CommandAllocator::Get()
{
	assert(m_pCommandAllocator != nullptr);
	return m_pCommandAllocator;
}

