#include "PCH.h"
#include "ConstantBufferView.h"


void cuc::ConstantBufferView::Init(ID3D12Device* pDevice, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size)
{
	assert(pDevice != nullptr);

	BufferLocation = location;
	SizeInBytes = (size + 255) & ~255; // 256-byte aligned
	D3D12_CPU_DESCRIPTOR_HANDLE dest;
	pDevice->CreateConstantBufferView(this, dest);
}
