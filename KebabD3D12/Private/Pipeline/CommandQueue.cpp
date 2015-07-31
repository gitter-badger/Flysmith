#include "PCH.h"
#include "CommandQueue.h"

cuc::CommandQueue::CommandQueue()
	: m_pCommandQueue(nullptr)
{
}

cuc::CommandQueue::~CommandQueue()
{
	if (m_pCommandQueue)
	{
		m_pCommandQueue->Release();
	}
}

void cuc::CommandQueue::Init(ID3D12Device* pDevice, 
							 CommandListType type, CommandQueuePriority priority, bool bDisableGPUTimeout, U32 nodeMask)
{
	assert(pDevice != nullptr);

	D3D12_COMMAND_QUEUE_DESC desc;
	desc.Type = static_cast<D3D12_COMMAND_LIST_TYPE>(type);
	desc.Priority = static_cast<D3D12_COMMAND_QUEUE_PRIORITY>(priority);
	desc.Flags = bDisableGPUTimeout ? D3D12_COMMAND_QUEUE_FLAG_DISABLE_GPU_TIMEOUT : D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask = nodeMask;

	HRESULT hr = pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_pCommandQueue));
	assert(SUCCEEDED(hr));
}

ID3D12CommandQueue* cuc::CommandQueue::Get()
{
	assert(m_pCommandQueue != nullptr);
	return m_pCommandQueue;
}
