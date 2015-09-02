#include "PCH.h"
#include "PipelineStateObject.h"


PipelineStateObject::PipelineStateObject()
	: m_pState(nullptr)
{
	ZeroMemory(&m_description, sizeof(m_description));
}

PipelineStateObject::PipelineStateObject(ID3D12Device* pDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) 
	: m_pState(nullptr)
	, m_description(desc)
{
	Init(pDevice, desc);
}

PipelineStateObject::PipelineStateObject(ID3D12Device* pDevice, const D3D12_INPUT_ELEMENT_DESC* layout, const U32 numLayoutElements,
										 ID3D12RootSignature* pRootSignature, const BlendStateConfig* pBlendState, const RasterizerStateConfig* pRasterizerState,
										 const D3D12_SHADER_BYTECODE * VS, const D3D12_SHADER_BYTECODE * PS, const D3D12_SHADER_BYTECODE * GS, 
										 const D3D12_SHADER_BYTECODE * HS, const D3D12_SHADER_BYTECODE * DS, const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType)
{
	Init(pDevice, layout, numLayoutElements, pRootSignature, pBlendState, pRasterizerState, VS, PS, GS, HS, DS, primitiveType);
}

PipelineStateObject::PipelineStateObject(PipelineStateObject& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
}

PipelineStateObject& PipelineStateObject::operator=(PipelineStateObject& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;

	return *this;
}

PipelineStateObject::PipelineStateObject(PipelineStateObject&& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
}

PipelineStateObject& PipelineStateObject::operator=(PipelineStateObject&& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;

	return *this;
}

PipelineStateObject::~PipelineStateObject()
{
	if (m_pState)
	{
		m_pState->Release();
		m_pState = nullptr;
	}
}

void PipelineStateObject::Init(ID3D12Device* pDevice, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc)
{
	HRESULT hr = pDevice->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_pState));
	assert(SUCCEEDED(hr));
}

void PipelineStateObject::Init(ID3D12Device* pDevice,
							   const D3D12_INPUT_ELEMENT_DESC* layout,
							   const U32 numLayoutElements,
							   ID3D12RootSignature* pRootSignature,
							   const BlendStateConfig* pBlendState,
							   const RasterizerStateConfig* pRasterizerState,
							   const D3D12_SHADER_BYTECODE* VS,
							   const D3D12_SHADER_BYTECODE* PS,
							   const D3D12_SHADER_BYTECODE* GS,
							   const D3D12_SHADER_BYTECODE* HS,
							   const D3D12_SHADER_BYTECODE* DS,
							   const D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType)
{
	assert(pDevice != nullptr);
	assert(numLayoutElements > 0);
	assert(layout != nullptr);

	// Root signature
	m_description.pRootSignature = pRootSignature;

	// Shader bytecode
	if(VS) m_description.VS = *VS;
	if(PS) m_description.PS = *PS;
	if(GS) m_description.GS = *GS;
	if(HS) m_description.HS = *HS;
	if(DS) m_description.DS = *DS;

	// Stream output buffer
	// ...

	// Blend state
	if(!pBlendState)
		m_description.BlendState = static_cast<D3D12_BLEND_DESC>(kDefaultBlendState);
	else
		m_description.BlendState = static_cast<D3D12_BLEND_DESC>(*pBlendState);
	m_description.SampleMask = UINT_MAX;

	// Rasterizer state
	if (!pRasterizerState)
		m_description.RasterizerState = kDefaultRasterizerState.Get();
	else
		m_description.RasterizerState = pRasterizerState->Get();

	// Depth-stencil state
	m_description.DepthStencilState.DepthEnable = FALSE;
	m_description.DepthStencilState.StencilEnable = FALSE;
	// ...

	// Input layout for the input assembler 
	m_description.InputLayout.pInputElementDescs = layout;
	m_description.InputLayout.NumElements = numLayoutElements;

	// Primitive
	m_description.PrimitiveTopologyType = primitiveType;

	// Render targets descriptors
	m_description.NumRenderTargets = 1;
	m_description.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	// MSAA disabled
	m_description.SampleDesc.Count = 1;
	m_description.SampleDesc.Quality = 0;

	Init(pDevice, m_description);
}

void PipelineStateObject::SetShader(ID3D12Device* pDevice, const ShaderType type, D3D12_SHADER_BYTECODE* pBytecode)
{
	assert(pBytecode != nullptr);

	switch (type)
	{
	case ShaderType::VERTEX_SHADER:
		m_description.VS = *pBytecode;
		break;
	case ShaderType::PIXEL_SHADER:
		m_description.PS = *pBytecode;
		break;
	case ShaderType::GEOMETRY_SHADER:
		m_description.GS = *pBytecode;
		break;
	case ShaderType::HULL_SHADER:
		m_description.HS = *pBytecode;
		break;
	case ShaderType::DOMAIN_SHADER:
		m_description.DS = *pBytecode;
		break;
	}

	Init(pDevice, m_description);
}

void PipelineStateObject::SetShader(ID3D12Device* pDevice, const ShaderType type, ID3DBlob* pShaderBlob)
{
	assert(pShaderBlob != nullptr);

	D3D12_SHADER_BYTECODE bytecode;
	bytecode.BytecodeLength = pShaderBlob->GetBufferSize();
	bytecode.pShaderBytecode = reinterpret_cast<BYTE*>(pShaderBlob->GetBufferPointer());

	SetShader(pDevice, type, &bytecode);
}

ID3D12PipelineState* PipelineStateObject::Get() const
{
	return m_pState;
}