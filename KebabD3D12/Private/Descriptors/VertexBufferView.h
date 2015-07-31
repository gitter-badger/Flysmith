#pragma once
#include "PublicDef.h"


namespace cuc
{
	struct VertexBufferView : D3D12_VERTEX_BUFFER_VIEW
	{
		// @location: Buffer's address.
		// @size: Buffer's size in bytes.
		// @stride: Size in bytes of each vertex entry.
		void Init(D3D12_GPU_VIRTUAL_ADDRESS location, U32 size, U32 stride);
	};
}