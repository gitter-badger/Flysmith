#pragma once
#include "KebabEngine.h"
#ifdef RENDERER_DX12
#include <d3d12.h>


namespace cuc
{
	enum class DescHeapType
	{
		// May contain constant-buffer, shader-resource and unordered-access views.
		CB_SR_UA         = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 
		// May contain samplers.
		SAMPLER          = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
		// May contain render-target views.
		RENDER_TARGET    = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		// May contain depth-stencil views.
		DEPTH_STENCIL    = D3D12_DESCRIPTOR_HEAP_TYPE_DSV
	};

	class DescriptorHeap
	{
	public:
		DescriptorHeap();
		~DescriptorHeap();

		// Will fail if the heap is already initialized.
		void Init(ID3D12Device*, DescHeapType);
		const bool IsInitialized() const;

		// Get a CPU handle to the descriptor within the heap at @index.
		// @index = 0 indicates the start of the heap.
		// Will fail if the heap is not initialized.
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(U32 index) const;

		// Get a GPU handle to the descriptor within the heap at @index.
		// @index = 0 indicates the start of the heap.
		// Will fail if the heap is not initialized.
		// Will fail if the heap is not shader/GPU visible(i.e. it is not either a CB_SR_UA or a SAMPLER heap).
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(U32 index) const;

	private:
		ID3D12DescriptorHeap* m_pDescHeap;
		D3D12_CPU_DESCRIPTOR_HANDLE m_startHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE m_startHandleGPU;
		U32 m_handleIncrement;
		bool m_bInitialized;
	};
}

#endif