#pragma once
#include "PCH.h"


enum class DescHeapType
{
	// May contain constant-buffer, shader-resource and unordered-access views.
	CB_SR_UA = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
	// May contain samplers.
	SAMPLER = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER,
	// May contain render-target views.
	RENDER_TARGET = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
	// May contain depth-stencil views.
	DEPTH_STENCIL = D3D12_DESCRIPTOR_HEAP_TYPE_DSV
};

class DescriptorHeap
{
public:
	DescriptorHeap();
	~DescriptorHeap();

	// Will fail if the heap is already initialized.
	// Creation of a shader visible heap(i.e. heap that can be accessed by the graphics pipeline/shaders) is possible only if the type is either CB_SR_UA or SAMPLER.
	void Init(ID3D12Device*, DescHeapType, U32 numDescriptors, bool bShaderVisible = false);

	const bool IsInitialized() const;
	const bool IsShaderVisible() const;

	// Get a CPU handle to the descriptor within the heap at @index.
	// @index = 0 indicates the start of the heap.
	// Will fail if the heap is not initialized.
	// Will fail if @index is not less than the number of descriptors in the heap.
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(U32 index) const;

	// Get a GPU handle to the descriptor within the heap at @index.
	// @index = 0 indicates the start of the heap.
	// Will fail if the heap is not initialized.
	// Will fail if @index is not less than the number of descriptors in the heap.
	// Will fail if the heap is not shader visible.
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(U32 index) const;

	ID3D12DescriptorHeap* Get();

private:
	ID3D12DescriptorHeap* m_pDescHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE m_startHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE m_startHandleGPU;
	U32 m_handleIncrement;
	U32 m_numDescriptors;
	bool m_bInitialized;
	bool m_bShaderVisible;
};