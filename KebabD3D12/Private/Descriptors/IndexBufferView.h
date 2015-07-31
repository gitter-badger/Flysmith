#pragma once
#include "PublicDef.h"


namespace cuc
{
	struct IndexBufferView : D3D12_INDEX_BUFFER_VIEW
	{
		// @location: Buffer's address.
		// @size: Buffer's size in bytes.
		void Init(D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, DXGI_FORMAT = DXGI_FORMAT_R32_UINT);
	};
}