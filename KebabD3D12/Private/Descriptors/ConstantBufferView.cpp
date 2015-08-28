#include "PCH.h"
#include "ConstantBufferView.h"


ConstantBufferView::ConstantBufferView()
{
}

ConstantBufferView::ConstantBufferView(ID3D12Device* pDevice, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, D3D12_CPU_DESCRIPTOR_HANDLE heapStartHandle)
{
	Init(pDevice, location, size, heapStartHandle);
}

void ConstantBufferView::Init(ID3D12Device* pDevice, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, D3D12_CPU_DESCRIPTOR_HANDLE heapStartHandle)
{
	assert(pDevice != nullptr);

	BufferLocation = location;
	SizeInBytes = (size + 255) & ~255; // 256-byte aligned
	pDevice->CreateConstantBufferView(this, heapStartHandle);
}
