#pragma once


enum class DescriptorRangeType
{
	SRV,     // Range of shader resource views.
	UAV,     // Range of unordered-access views.
	CBV,     // Range of constant-buffer views.
	SAMPLER  // Range of samplers. 
};

struct DescriptorRange : D3D12_DESCRIPTOR_RANGE
{
	// @numDescriptors: The number of descriptors in the range. 
	//					D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND (-1) indicates unbounded size. 
    //					Only the last entry in a table can have unbounded size. 
	//
	// @baseShaderRegister: The base shader register in the range. 
	//						E.g. for SRVs, 3 maps to ": register(t3);" in HLSL. 
	//
	// @offset : The offset in descriptors from the start of the root signature. 
	//			 D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND (-1) indicates unbounded size. 
	//
	// @registerSpace: The register space. Can typically be 0, but allows multiple descriptor arrays of unknown size to not appear to overlap. 
	//				   E.g. for SRVs, by extending the example in the BaseShaderRegister member description, 5 maps to ": register(t3,space5);" in HLSL. 
	DescriptorRange(DescriptorRangeType, U32 numDescriptors, U32 baseShaderRegister, U32 offset, U32 registerSpace = 0);
};