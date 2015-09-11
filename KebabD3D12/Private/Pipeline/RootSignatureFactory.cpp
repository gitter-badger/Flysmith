#include "PCH.h"
#include "RootSignatureFactory.h"
using namespace Microsoft::WRL;


RootParameter::RootParameter(const RootParameterType type, const D3D12_SHADER_VISIBILITY shaderVisibility)
{
	ZeroMemory(this, sizeof(this));
	ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(type);
	ShaderVisibility = shaderVisibility;
}

RootSignatureFactory::RootSignatureFactory(const RootSignatureFlag flags)
	: m_flags(flags)
	, m_rootSize(0)
{
}

RootSignatureFactory::~RootSignatureFactory()
{
}

void RootSignatureFactory::SetFlags(const RootSignatureFlag flags)
{
	m_flags = flags;
}

U32 RootSignatureFactory::AddParameter(const RootParameter& parameter)
{
	U32 cost = 0;
	switch (parameter.ParameterType)
	{
	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
		cost = parameter.Constants.Num32BitValues;
		break;
	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
		cost = 1;
		break;
	case D3D12_ROOT_PARAMETER_TYPE_CBV:
	case D3D12_ROOT_PARAMETER_TYPE_SRV:
	case D3D12_ROOT_PARAMETER_TYPE_UAV:
		cost = 4;
		break;
	}

	assert(m_rootSize + cost <= 16);
	m_rootSize += cost;

	m_parameters.push_back(parameter);
	return m_parameters.size() - 1;
}

U32 RootSignatureFactory::AddParameterConstants(const U32 numValues, const U32 shaderRegister, const D3D12_SHADER_VISIBILITY shaderVisibility, const U32 registerSpace)
{
	assert(m_rootSize + numValues <= 16);
	m_rootSize += numValues;

	D3D12_ROOT_PARAMETER parameter;
	parameter.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(RootParameterType::INL_CONSTANTS);
	parameter.ShaderVisibility = shaderVisibility;
	parameter.Constants.Num32BitValues = numValues;
	parameter.Constants.ShaderRegister = shaderRegister;
	parameter.Constants.RegisterSpace = registerSpace;

	m_parameters.push_back(parameter);
	return m_parameters.size() - 1;
}

U32 RootSignatureFactory::AddParameterDescriptor(const RootParameterType type, const U32 shaderRegister, const D3D12_SHADER_VISIBILITY shaderVisibility, const U32 registerSpace)
{
	assert(m_rootSize + 4 <= 16);
	m_rootSize += 4;

	D3D12_ROOT_PARAMETER parameter;
	parameter.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(type);
	parameter.ShaderVisibility = shaderVisibility;
	parameter.Descriptor.ShaderRegister = shaderRegister;
	parameter.Descriptor.RegisterSpace = registerSpace;

	m_parameters.push_back(parameter);
	return m_parameters.size() - 1;
}

U32 RootSignatureFactory::AddParameterDescTable(const U32 numDescriptorRanges, const D3D12_DESCRIPTOR_RANGE* pDescriptorRanges, const D3D12_SHADER_VISIBILITY shaderVisibility)
{
	assert(m_rootSize + 1 <= 16);
	m_rootSize += 1;

	D3D12_ROOT_PARAMETER parameter;
	parameter.ParameterType = static_cast<D3D12_ROOT_PARAMETER_TYPE>(RootParameterType::DESCRIPTOR_TABLE);
	parameter.ShaderVisibility = shaderVisibility;
	parameter.DescriptorTable.NumDescriptorRanges = numDescriptorRanges;
	parameter.DescriptorTable.pDescriptorRanges = pDescriptorRanges;

	m_parameters.push_back(parameter);
	return m_parameters.size() - 1;
}

void RootSignatureFactory::AddStaticSampler(const D3D12_STATIC_SAMPLER_DESC& staticSampler)
{
	m_staticSamplers.push_back(staticSampler);
}

U32 RootSignatureFactory::GetSize()
{
	return m_rootSize;
}

void RootSignatureFactory::BuildRootSignature(ID3D12Device* pDevice, RootSignature* pOut)
{
	assert(pDevice != nullptr);

	D3D12_ROOT_PARAMETER* paramArrayPtr = m_parameters.size() ? &m_parameters[0] : nullptr;
	D3D12_STATIC_SAMPLER_DESC* samplerArrayPtr = m_staticSamplers.size() ? &m_staticSamplers[0] : nullptr;

	D3D12_ROOT_SIGNATURE_DESC description;
	description.NumParameters = m_parameters.size();
	description.pParameters = paramArrayPtr;
	description.NumStaticSamplers = m_staticSamplers.size();
	description.pStaticSamplers = samplerArrayPtr;
	description.Flags = static_cast<D3D12_ROOT_SIGNATURE_FLAGS>(m_flags);

	ID3DBlob* pOutBlob = nullptr;
	ID3DBlob* pErrorBlob = nullptr;

	HRESULT hr = D3D12SerializeRootSignature(&description, D3D_ROOT_SIGNATURE_VERSION_1, &pOutBlob, &pErrorBlob);
	assert(SUCCEEDED(hr));

	if (pErrorBlob)
	{
		__debugbreak();
		pErrorBlob->Release();
	}

	ID3D12RootSignature* pRootSignature;

	hr = pDevice->CreateRootSignature(0,
									  pOutBlob->GetBufferPointer(),
								      pOutBlob->GetBufferSize(),
									  IID_PPV_ARGS(&pRootSignature));
	assert(SUCCEEDED(hr));

	pOutBlob->Release();

	pOut->Init(pRootSignature);
}