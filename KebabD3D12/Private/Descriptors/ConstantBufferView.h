#pragma once
#include "PublicDef.h"


namespace cuc
{
	struct ConstantBufferView : D3D12_CONSTANT_BUFFER_VIEW_DESC
	{
		// @location: Buffer's address.
		// @size: Buffer's size in bytes.
		void Init(ID3D12Device*, D3D12_GPU_VIRTUAL_ADDRESS location, U32 size);
	};
}