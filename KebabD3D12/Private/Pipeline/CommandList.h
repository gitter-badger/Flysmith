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

		// @commandAllocator: Cannot be associated with another command list.
		// @initialState: Not inherited from previous command list.
		void Reset(ID3D12CommandAllocator*, ID3D12PipelineState* pInitialState = nullptr);
		void Close();
		
		void SetRootSignature(ID3D12RootSignature*);
		void SetViewports(D3D12_VIEWPORT*, U32 numViewports = 1);
		void SetScissorRects(D3D12_RECT*, U32 numRects = 1);
		void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY);
		void SetVertexBuffers(D3D12_VERTEX_BUFFER_VIEW*, U32 numViews = 1, U32 startSlot = 0);
		void SetIndexBuffer(D3D12_INDEX_BUFFER_VIEW*);
		
		void SetRoot32BitConstants(U32 rootParamIndex, U32 numValues, void* pData, U32 offsetIn32BitValues);
		
		void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, F32* cleaColor, D3D12_RECT*, U32 numRects = 1);
		void SetRenderTargets(U32 numRenderTargetDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE* pRTVDescriptors, 
							  BOOL bSingleHandleToDescriptorRange, D3D12_CPU_DESCRIPTOR_HANDLE* pDepthStencilDescriptor);
		
		void DrawIndexedInstanced(U32 numPerInstance, U32 numInstances = 1, U32 startIndexLoc = 0, I32 baseVertLoc = 0, U32 startInstanceLoc = 0);
		void DrawIndexed(U32 numIndices, U32 startIndexLoc = 0, I32 baseVertLoc = 0);
		void Draw(U32 numVertices, U32 startVertexLoc = 0);

	private:
		ID3D12GraphicsCommandList* m_pCommandList;
	};
}