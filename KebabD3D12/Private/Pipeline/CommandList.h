#pragma once


namespace cuc
{
	enum CommandListType
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

	class CommandList
	{
	public:
		CommandList();
		~CommandList();
		void Init(ID3D12Device*, ID3D12CommandAllocator*, ID3D12PipelineState* pInitialState = nullptr, CommandListType = DIRECT, U32 nodeMask = 0);
		ID3D12GraphicsCommandList* Get();

	private:
		ID3D12GraphicsCommandList* m_pCommandList;
	};
}