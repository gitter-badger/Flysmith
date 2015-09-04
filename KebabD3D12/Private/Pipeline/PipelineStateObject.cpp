#include "PCH.h"
#include "PipelineStateObject.h"


ShaderPipeline::ShaderPipeline(const D3D12_SHADER_BYTECODE* VS_, const D3D12_SHADER_BYTECODE* PS_, const D3D12_SHADER_BYTECODE* GS_,
							   const D3D12_SHADER_BYTECODE* HS_, const D3D12_SHADER_BYTECODE* DS_)
	: VS(VS_), PS(PS_), GS(GS_), HS(HS_), DS(DS_)
{
}

PipelineStateObject::PipelineStateObject()
	: m_pState(nullptr)
	, m_bInitialized(false)
{
	ZeroMemory(&m_description, sizeof(m_description));
}

PipelineStateObject::PipelineStateObject(PipelineStateObject& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
	m_bInitialized = other.m_bInitialized;
}

PipelineStateObject& PipelineStateObject::operator=(PipelineStateObject& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
	m_bInitialized = other.m_bInitialized;

	return *this;
}

PipelineStateObject::PipelineStateObject(PipelineStateObject&& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
	m_bInitialized = other.m_bInitialized;
}

PipelineStateObject& PipelineStateObject::operator=(PipelineStateObject&& other)
{
	other.m_pState->AddRef();
	m_pState = other.m_pState;
	m_description = other.m_description;
	m_bInitialized = other.m_bInitialized;

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

void PipelineStateObject::Init(ID3D12Device* pDevice,
							   const D3D12_INPUT_ELEMENT_DESC* layout,
							   const U32 numLayoutElements,
							   const ShaderPipeline& shaderPipeline,
							   ID3D12RootSignature* pRootSignature,
							   const RasterizerStateConfig* pRasterizerState,
							   const BlendStateConfig* pBlendState,
							   PrimitiveTopologyType primitiveType)
{
	assert(!m_bInitialized);
	assert(pDevice != nullptr);
	assert(numLayoutElements > 0);
	assert(layout != nullptr);

	// Root signature
	m_description.pRootSignature = pRootSignature;

	// Shader bytecode
	if(shaderPipeline.VS) m_description.VS = *shaderPipeline.VS;
	if(shaderPipeline.PS) m_description.PS = *shaderPipeline.PS;
	if(shaderPipeline.GS) m_description.GS = *shaderPipeline.GS;
	if(shaderPipeline.HS) m_description.HS = *shaderPipeline.HS;
	if(shaderPipeline.DS) m_description.DS = *shaderPipeline.DS;

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
	m_description.DepthStencilState.DepthEnable = TRUE;
	m_description.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	m_description.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	m_description.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	m_description.DepthStencilState.StencilEnable = FALSE;
	// ...

	// Input layout for the input assembler 
	m_description.InputLayout.pInputElementDescs = layout;
	m_description.InputLayout.NumElements = numLayoutElements;

	// Primitive
	m_description.PrimitiveTopologyType = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(primitiveType);

	// Render targets descriptors
	m_description.NumRenderTargets = 1;
	m_description.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	// MSAA disabled
	m_description.SampleDesc.Count = 1;
	m_description.SampleDesc.Quality = 0;

	HRESULT hr = pDevice->CreateGraphicsPipelineState(&m_description, IID_PPV_ARGS(&m_pState));
	assert(SUCCEEDED(hr));

	m_bInitialized = true;
}

FillMode PipelineStateObject::GetFillMode() const
{
	return static_cast<FillMode>(m_description.RasterizerState.FillMode);
}

CullMode PipelineStateObject::GetCullMode() const
{
	return static_cast<CullMode>(m_description.RasterizerState.CullMode);
}

const void* PipelineStateObject::GetVertexShader() const
{
	return m_description.VS.pShaderBytecode;
}

const void* PipelineStateObject::GetPixelShader() const
{
	return m_description.PS.pShaderBytecode;
}

ID3D12PipelineState* PipelineStateObject::Get() const
{
	return m_pState;
}

bool operator==(PipelineStateObject& lhs, PipelineStateObject& rhs)
{
	auto& pso1 = lhs.m_description,
		  pso2 = rhs.m_description;

	if ((pso1.pRootSignature == pso2.pRootSignature) &&
		(pso1.RasterizerState.CullMode == pso2.RasterizerState.CullMode) &&
		(pso1.RasterizerState.FillMode == pso2.RasterizerState.FillMode) &&
		(pso1.VS.BytecodeLength == pso2.VS.BytecodeLength) &&
		(pso1.PS.BytecodeLength == pso2.PS.BytecodeLength))
	{
		return true;
	}

	return false;
}