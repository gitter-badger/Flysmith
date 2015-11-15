#pragma once


struct ConstantBufferView : D3D12_CONSTANT_BUFFER_VIEW_DESC
{
	ConstantBufferView();
	// @location: Buffer's address.
	// @size: Buffer's size in bytes.
	// @heapStartHandle: The CPU descriptor handle that represents the start of the heap that holds the constant-buffer view. 
	ConstantBufferView(ID3D12Device*, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, D3D12_CPU_DESCRIPTOR_HANDLE heapStartHandle);

	// @location: Buffer's address.
	// @size: Buffer's size in bytes.
	// @heapStartHandle: The CPU descriptor handle that represents the start of the heap that holds the constant-buffer view. 
	void Init(ID3D12Device*, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, D3D12_CPU_DESCRIPTOR_HANDLE heapStartHandle);
};