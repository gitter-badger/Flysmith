#include "PCH.h"
#include "DescriptorRange.h"


DescriptorRange::DescriptorRange(DescriptorRangeType type, U32 numDescriptors, U32 baseShaderRegister, U32 offset, U32 registerSpace)
{
	RangeType = static_cast<D3D12_DESCRIPTOR_RANGE_TYPE>(type);
	NumDescriptors = numDescriptors;
	BaseShaderRegister = baseShaderRegister;
	OffsetInDescriptorsFromTableStart = offset;
	RegisterSpace = registerSpace;
}