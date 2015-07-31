#pragma once
#include "CommandList.h"


struct ID3D12CommandQueue;
struct ID3D12Device;

namespace cuc
{
	// TODO: Look into what it actually means.
	enum CommandQueuePriority
	{
		NORMAL = 0,
		HIGH   = 100
	};

	class CommandQueue
	{
	public:
		CommandQueue();
		~CommandQueue();
		void Init(ID3D12Device*, CommandListType = DIRECT, CommandQueuePriority = NORMAL, bool bDisableGPUTimeout = false, U32 nodeMask = 0);
		ID3D12CommandQueue* Get();

	private:
		ID3D12CommandQueue* m_pCommandQueue;
	};
}