#pragma once
#include "CommandQueue.h"


struct ID3D12CommandQueue;
struct ID3D12Device;

namespace cuc
{
	enum class CommandListType
	{
		// Can be executed by the GPU. Doesn't inherit any GPU state.
		DIRECT,
		// Can be executed only via a direct command list. Inherits all GPU state, except for the PSO and primitive topology.
		BUNDLE,
		// Command buffer for computing.
		COMPUTE,
		// Command buffer for copying. 
		// TODO: Look into what it actually means.
		COPY
	};

	// TODO: Look into what it actually means.
	enum class CommandQueuePriority
	{
		NORMAL = 0,
		HIGH   = 100
	};

	class CommandQueue
	{
	public:
		CommandQueue();
		~CommandQueue();
		void Init(ID3D12Device*, CommandListType, CommandQueuePriority, bool bDisableGPUTimeout = false, U32 nodeMask = 0);
		ID3D12CommandQueue* Get();

	private:
		ID3D12CommandQueue* m_pCommandQueue;
	};
}