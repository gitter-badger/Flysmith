#include "PCH.h"
#include "IndexBufferView.h"


void IndexBufferView::Init(D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, DXGI_FORMAT format)
{
	BufferLocation = location;
	SizeInBytes = size;
	Format = format;
}
