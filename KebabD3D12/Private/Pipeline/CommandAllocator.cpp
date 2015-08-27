#include "PCH.h"
#include "CommandAllocator.h"


CommandAllocator::CommandAllocator()
	: m_pCommandAllocator(nullptr)
{
}

CommandAllocator::~CommandAllocator()
{
	if (m_pCommandAllocator)
	{
		m_pCommandAllocator->Release();
	}
}

void CommandAllocator::Init(ID3D12Device* pDevice, CommandListType type)
{
	assert(pDevice != nullptr);
	assert(m_pCommandAllocator == nullptr);

	HRESULT hr = pDevice->CreateCommandAllocator(static_cast<D3D12_COMMAND_LIST_TYPE>(type), IID_PPV_ARGS(&m_pCommandAllocator));
	assert(SUCCEEDED(hr));
}

ID3D12CommandAllocator* CommandAllocator::Get()
{
	assert(m_pCommandAllocator != nullptr);
	return m_pCommandAllocator;
}

void CommandAllocator::Reset()
{
	HRESULT hr = m_pCommandAllocator->Reset();
	assert(SUCCEEDED(hr));
}

