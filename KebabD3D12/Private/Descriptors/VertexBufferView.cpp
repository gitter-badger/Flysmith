#include "PCH.h"
#include "VertexBufferView.h"


void cuc::VertexBufferView::Init(D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, U32 stride)
{
	BufferLocation = location;
	StrideInBytes = stride;
	SizeInBytes = size;
}
