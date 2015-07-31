#include "PCH.h"
#include "CommandList.h"

cuc::CommandList::CommandList()
	: m_pCommandList(nullptr)
{
}

cuc::CommandList::~CommandList()
{
	if (m_pCommandList)
	{
		m_pCommandList->Release();
	}
}

void cuc::CommandList::Init(ID3D12Device* pDevice, ID3D12CommandAllocator* pAllocator, ID3D12PipelineState* pInitialState, 
							CommandListType type, U32 nodeMask)
{
	assert(pDevice != nullptr);
	assert(pAllocator != nullptr);
	assert(m_pCommandList == nullptr);

	HRESULT hr = pDevice->CreateCommandList(nodeMask, static_cast<D3D12_COMMAND_LIST_TYPE>(type), 
											pAllocator, pInitialState, 
											IID_PPV_ARGS(&m_pCommandList));
	assert(SUCCEEDED(hr));
}

ID3D12GraphicsCommandList* cuc::CommandList::Get()
{
	assert(m_pCommandList != nullptr);
	return m_pCommandList;
}
