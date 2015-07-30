#pragma once
#include "PCH.h"


namespace cuc
{
	enum class RootParameterType
	{
		DESCRIPTOR_TABLE      = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE, // Cost: 1 DWORD
		INL_CONSTANTS         = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS,  // Cost: Number of constants DWORDs
		INL_CONSTANT_BUFFER   = D3D12_ROOT_PARAMETER_TYPE_CBV,              // Cost: 4 DWORDs
		INL_SHADER_RESOURCE   = D3D12_ROOT_PARAMETER_TYPE_SRV,              // Cost: 4 DWORDs
		INL_UNORDERED_ACCESS  = D3D12_ROOT_PARAMETER_TYPE_UAV               // Cost: 4 DWORDs
	};

	struct RootParameter : D3D12_ROOT_PARAMETER
	{
		RootParameter(const RootParameterType, const D3D12_SHADER_VISIBILITY = D3D12_SHADER_VISIBILITY_ALL);
	};

	// One graphics and one compute root signature per application.
	// Root signatures have a 16 DWORD size limit.
	class RootSignatureFactory
	{
	public:
		enum RootSignatureFlag
		{
			// Defualt behavior
			NONE                   = D3D12_ROOT_SIGNATURE_FLAG_NONE,                                
			// Allow IA input layout
			ALLOW_IA_LAYOUT        = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,  
			DENY_ACCESS_VS         = D3D12_ROOT_SIGNATURE_FLAG_DENY_VERTEX_SHADER_ROOT_ACCESS,      
			DENY_ACCESS_HS         = D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS,        
			DENY_ACCESS_DS         = D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS,      
			DENY_ACCESS_GS         = D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS,    
			DENY_ACCESS_PS         = D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS,       
			// Allow stream output. Creating a PSO with a GS containing stream-output will fail if this flag is not set. 
			ALLOW_STREAM_OUTPUT    = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_STREAM_OUTPUT                  
		};
		
	public:
		RootSignatureFactory(const RootSignatureFlag flags = NONE);
		~RootSignatureFactory();
		
		void SetFlags(const RootSignatureFlag flags);

		// Returns the added parameter's slot.
		// Fails if the new parameter can't fit in the root signature.
		U32 AddParameter(const RootParameter&);

		// Incurs a @numValues DWORDs cost.
		// Fails if the new parameter can't fit in the root signature.
		U32 AddParameterConstants(const U32 numValues,
								  const U32 shaderRegister = 0,
								  const D3D12_SHADER_VISIBILITY = D3D12_SHADER_VISIBILITY_ALL,
								  const U32 registerSpace = 0); // Leave 0 if using a shader targeting an older model than 5.1

		// Incurs a 4 DWORDs cost.
		// Fails if the new parameter can't fit in the root signature.
		U32 AddParameterDescriptor(const RootParameterType type, 
								   const U32 shaderRegister = 0,
								   const D3D12_SHADER_VISIBILITY = D3D12_SHADER_VISIBILITY_ALL,
								   const U32 registerSpace = 0); // Leave 0 if using a shader targeting an older model than 5.1

		// Incurs a 4 DWORDs cost.
		// Fails if the new parameter can't fit in the root signature.
		U32 AddParameterDescTable(const U32 numDescriptorRanges,
								  const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges,
								  const D3D12_SHADER_VISIBILITY = D3D12_SHADER_VISIBILITY_ALL);

		void AddStaticSampler(const D3D12_STATIC_SAMPLER_DESC&);

		// Get the current size in DWORDs
		U32 GetSize();

		ID3D12RootSignature* BuildRootSignature(ID3D12Device*);
	
	private:
		U32 m_rootSize; // In DWORDs
		RootSignatureFlag m_flags;
		std::vector<D3D12_ROOT_PARAMETER> m_parameters;
		std::vector<D3D12_STATIC_SAMPLER_DESC> m_staticSamplers;
	};
}